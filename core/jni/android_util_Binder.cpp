/*
 * Copyright (C) 2006 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#define LOG_TAG "JavaBinder"
//#define LOG_NDEBUG 0

#include "android_os_Parcel.h"
#include "android_util_Binder.h"

#include "JNIHelp.h"

#include <fcntl.h>
#include <inttypes.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <utils/Atomic.h>
#include <utils/Log.h>
#include <utils/SystemClock.h>
#include <utils/List.h>
#include <utils/KeyedVector.h>
#include <log/logger.h>
#include <binder/Parcel.h>
#include <binder/IBinder.h>
#include <utils/threads.h>
#include <utils/String8.h>

#include <ScopedUtfChars.h>
#include <ScopedLocalRef.h>

#include <android_runtime/AndroidRuntime.h>

//#undef ALOGV
//#define ALOGV(...) fprintf(stderr, __VA_ARGS__)

#define DEBUG_DEATH 0
#if DEBUG_DEATH
#define LOGDEATH ALOGD
#else
#define LOGDEATH ALOGV
#endif

using namespace android;

// ----------------------------------------------------------------------------

static struct bindernative_offsets_t
{
    // Class state.
    jclass mClass;
    jmethodID mExecTransact;

    // Object state.
    jfieldID mObject;

} gBinderOffsets;

// ----------------------------------------------------------------------------

static struct binderinternal_offsets_t
{
    // Class state.
    jclass mClass;
    jmethodID mForceGc;

} gBinderInternalOffsets;

// ----------------------------------------------------------------------------

static struct debug_offsets_t
{
    // Class state.
    jclass mClass;

} gDebugOffsets;

// ----------------------------------------------------------------------------

static struct error_offsets_t
{
    jclass mClass;
} gErrorOffsets;

// ----------------------------------------------------------------------------

static struct binderproxy_offsets_t
{
    // Class state.
    jclass mClass;
    jmethodID mConstructor;
    jmethodID mSendDeathNotice;

    // Object state.
    jfieldID mObject;
    jfieldID mSelf;
    jfieldID mOrgue;

} gBinderProxyOffsets;

static struct class_offsets_t
{
    jmethodID mGetName;
} gClassOffsets;

// ----------------------------------------------------------------------------

static struct log_offsets_t
{
    // Class state.
    jclass mClass;
    jmethodID mLogE;
} gLogOffsets;

static struct parcel_file_descriptor_offsets_t
{
    jclass mClass;
    jmethodID mConstructor;
} gParcelFileDescriptorOffsets;

static struct strict_mode_callback_offsets_t
{
    jclass mClass;
    jmethodID mCallback;
} gStrictModeCallbackOffsets;

// ****************************************************************************
// ****************************************************************************
// ****************************************************************************

static volatile int32_t gNumRefsCreated = 0;
static volatile int32_t gNumProxyRefs = 0;
static volatile int32_t gNumLocalRefs = 0;
static volatile int32_t gNumDeathRefs = 0;

static void incRefsCreated(JNIEnv* env)
{
    int old = android_atomic_inc(&gNumRefsCreated);
    if (old == 200) {
        android_atomic_and(0, &gNumRefsCreated);
        env->CallStaticVoidMethod(gBinderInternalOffsets.mClass,
                gBinderInternalOffsets.mForceGc);
    } else {
        ALOGV("Now have %d binder ops", old);
    }
}

static JavaVM* jnienv_to_javavm(JNIEnv* env)
{
    JavaVM* vm;
    return env->GetJavaVM(&vm) >= 0 ? vm : NULL;
}

static JNIEnv* javavm_to_jnienv(JavaVM* vm)
{
    JNIEnv* env;
    return vm->GetEnv((void **)&env, JNI_VERSION_1_4) >= 0 ? env : NULL;
}

static void report_exception(JNIEnv* env, jthrowable excep, const char* msg)
{
    env->ExceptionClear();

    jstring tagstr = env->NewStringUTF(LOG_TAG);
    jstring msgstr = env->NewStringUTF(msg);

    if ((tagstr == NULL) || (msgstr == NULL)) {
        env->ExceptionClear();      /* assume exception (OOM?) was thrown */
        ALOGE("Unable to call Log.e()\n");
        ALOGE("%s", msg);
        goto bail;
    }

    env->CallStaticIntMethod(
        gLogOffsets.mClass, gLogOffsets.mLogE, tagstr, msgstr, excep);
    if (env->ExceptionCheck()) {
        /* attempting to log the failure has failed */
        ALOGW("Failed trying to log exception, msg='%s'\n", msg);
        env->ExceptionClear();
    }

    if (env->IsInstanceOf(excep, gErrorOffsets.mClass)) {
        /*
         * It's an Error: Reraise the exception, detach this thread, and
         * wait for the fireworks. Die even more blatantly after a minute
         * if the gentler attempt doesn't do the trick.
         *
         * The GetJavaVM function isn't on the "approved" list of JNI calls
         * that can be made while an exception is pending, so we want to
         * get the VM ptr, throw the exception, and then detach the thread.
         */
        JavaVM* vm = jnienv_to_javavm(env);
        env->Throw(excep);
        vm->DetachCurrentThread();
        sleep(60);
        ALOGE("Forcefully exiting");
        exit(1);
        *((int *) 1) = 1;
    }

bail:
    /* discard local refs created for us by VM */
    env->DeleteLocalRef(tagstr);
    env->DeleteLocalRef(msgstr);
}

class JavaBBinder : public IBinder
{
public:
    JavaBBinder(JNIEnv* env, jobject object)
        : mVM(jnienv_to_javavm(env)), mObject(env->NewGlobalRef(object))
    {
        ALOGV("Creating JavaBBinder %p\n", this);
        android_atomic_inc(&gNumLocalRefs);
        incRefsCreated(env);
    }

    bool    checkSubclass(const void* subclassID) const
    {
        return subclassID == &gBinderOffsets;
    }

    jobject object() const
    {
        return mObject;
    }
	
	virtual status_t transact(   uint32_t code,
                                        const Parcel& data,
                                        Parcel* reply,
                                        uint32_t flags = 0);

    virtual void attachObject(   const void* objectID,
                                            void* object,
                                            void* cleanupCookie,
                                            object_cleanup_func func);
    virtual void* findObject(const void* objectID) const ;
    virtual void detachObject(const void* objectID) ;

protected:
    virtual ~JavaBBinder()
    {
        ALOGV("Destroying JavaBBinder %p\n", this);
        android_atomic_dec(&gNumLocalRefs);
        JNIEnv* env = javavm_to_jnienv(mVM);
        env->DeleteGlobalRef(mObject);
    }

    virtual status_t onTransact(
        uint32_t code, const Parcel& data, Parcel* reply, uint32_t flags = 0)
    {
        JNIEnv* env = javavm_to_jnienv(mVM);

        ALOGV("onTransact() on %p calling object %p in env %p vm %p\n", this, mObject, env, mVM);

        return UNKNOWN_TRANSACTION;
    }

    virtual status_t dump(int fd, const Vector<String16>& args)
    {
        return 0;
    }

private:
    JavaVM* const   mVM;
    jobject const   mObject;
};


status_t JavaBBinder::transact(uint32_t code, const Parcel& data, Parcel* reply, uint32_t flags)
{

	return onTransact(code, data, reply, flags);
}
void JavaBBinder::attachObject(const void* objectID, void* object, void* cleanupCookie, object_cleanup_func func) 
{
}
void* JavaBBinder::findObject(const void* objectID) const 
{

	return NULL;
}
void  JavaBBinder::detachObject(const void* objectID) 
{


}


// ----------------------------------------------------------------------------

class JavaBBinderHolder : public RefBase
{
public:
    sp<JavaBBinder> get(JNIEnv* env, jobject obj)
    {
        AutoMutex _l(mLock);
        sp<JavaBBinder> b = mBinder.promote();
        if (b == NULL) {
            b = new JavaBBinder(env, obj);
            mBinder = b;
            ALOGV("Creating JavaBinder %p (refs %p) for Object %p, weakCount=%" PRId32 "\n",
                 b.get(), b->getWeakRefs(), obj, b->getWeakRefs()->getWeakCount());
        }

        return b;
    }

    sp<JavaBBinder> getExisting()
    {
        AutoMutex _l(mLock);
        return mBinder.promote();
    }

private:
    Mutex           mLock;
    wp<JavaBBinder> mBinder;
};

// ----------------------------------------------------------------------------

namespace android {


jobject newParcelFileDescriptor(JNIEnv* env, jobject fileDesc)
{
    return env->NewObject(
            gParcelFileDescriptorOffsets.mClass, gParcelFileDescriptorOffsets.mConstructor, fileDesc);
}

void set_dalvik_blockguard_policy(JNIEnv* env, jint strict_policy)
{
    // Call back into android.os.StrictMode#onBinderStrictModePolicyChange
    // to sync our state back to it.  See the comments in StrictMode.java.
    env->CallStaticVoidMethod(gStrictModeCallbackOffsets.mClass,
                              gStrictModeCallbackOffsets.mCallback,
                              strict_policy);
}

void signalExceptionForError(JNIEnv* env, jobject obj, status_t err,
        bool canThrowRemoteException)
{
    switch (err) {
        case UNKNOWN_ERROR:
            jniThrowException(env, "java/lang/RuntimeException", "Unknown error");
            break;
        case NO_MEMORY:
            jniThrowException(env, "java/lang/OutOfMemoryError", NULL);
            break;
        case INVALID_OPERATION:
            jniThrowException(env, "java/lang/UnsupportedOperationException", NULL);
            break;
        case BAD_VALUE:
            jniThrowException(env, "java/lang/IllegalArgumentException", NULL);
            break;
        case BAD_INDEX:
            jniThrowException(env, "java/lang/IndexOutOfBoundsException", NULL);
            break;
        case BAD_TYPE:
            jniThrowException(env, "java/lang/IllegalArgumentException", NULL);
            break;
        case NAME_NOT_FOUND:
            jniThrowException(env, "java/util/NoSuchElementException", NULL);
            break;
        case PERMISSION_DENIED:
            jniThrowException(env, "java/lang/SecurityException", NULL);
            break;
        case NOT_ENOUGH_DATA:
            jniThrowException(env, "android/os/ParcelFormatException", "Not enough data");
            break;
        case NO_INIT:
            jniThrowException(env, "java/lang/RuntimeException", "Not initialized");
            break;
        case ALREADY_EXISTS:
            jniThrowException(env, "java/lang/RuntimeException", "Item already exists");
            break;
        case DEAD_OBJECT:
            // DeadObjectException is a checked exception, only throw from certain methods.
            jniThrowException(env, canThrowRemoteException
                    ? "android/os/DeadObjectException"
                            : "java/lang/RuntimeException", NULL);
            break;
        case UNKNOWN_TRANSACTION:
            jniThrowException(env, "java/lang/RuntimeException", "Unknown transaction code");
            break;
        case FAILED_TRANSACTION:
            ALOGE("!!! FAILED BINDER TRANSACTION !!!");
            // TransactionTooLargeException is a checked exception, only throw from certain methods.
            // FIXME: Transaction too large is the most common reason for FAILED_TRANSACTION
            //        but it is not the only one.  The Binder driver can return BR_FAILED_REPLY
            //        for other reasons also, such as if the transaction is malformed or
            //        refers to an FD that has been closed.  We should change the driver
            //        to enable us to distinguish these cases in the future.
            jniThrowException(env, canThrowRemoteException
                    ? "android/os/TransactionTooLargeException"
                            : "java/lang/RuntimeException", NULL);
            break;
        case FDS_NOT_ALLOWED:
            jniThrowException(env, "java/lang/RuntimeException",
                    "Not allowed to write file descriptors here");
            break;
        default:
            ALOGE("Unknown binder error code. 0x%x", err);
            String8 msg;
            msg.appendFormat("Unknown binder error code. 0x%x", err);
            // RemoteException is a checked exception, only throw from certain methods.
            jniThrowException(env, canThrowRemoteException
                    ? "android/os/RemoteException" : "java/lang/RuntimeException", msg.string());
            break;
    }
}

}

// ----------------------------------------------------------------------------

static jint android_os_Binder_getCallingPid(JNIEnv* env, jobject clazz)
{
    return 0;
}

static jint android_os_Binder_getCallingUid(JNIEnv* env, jobject clazz)
{
    return 0;
}

static jlong android_os_Binder_clearCallingIdentity(JNIEnv* env, jobject clazz)
{
    return 0;
}

static void android_os_Binder_restoreCallingIdentity(JNIEnv* env, jobject clazz, jlong token)
{
    // XXX temporary sanity check to debug crashes.
    int uid = (int)(token>>32);
    if (uid > 0 && uid < 999) {
        // In Android currently there are no uids in this range.
        char buf[128];
        sprintf(buf, "Restoring bad calling ident: 0x%" PRIx64, token);
        jniThrowException(env, "java/lang/IllegalStateException", buf);
        return;
    }
    //IPCThreadState::self()->restoreCallingIdentity(token);
}

static void android_os_Binder_setThreadStrictModePolicy(JNIEnv* env, jobject clazz, jint policyMask)
{
    //IPCThreadState::self()->setStrictModePolicy(policyMask);
}

static jint android_os_Binder_getThreadStrictModePolicy(JNIEnv* env, jobject clazz)
{
    return 0;
}

static void android_os_Binder_flushPendingCommands(JNIEnv* env, jobject clazz)
{
    //IPCThreadState::self()->flushCommands();
}

static void android_os_Binder_init(JNIEnv* env, jobject obj)
{
    JavaBBinderHolder* jbh = new JavaBBinderHolder();
    if (jbh == NULL) {
        jniThrowException(env, "java/lang/OutOfMemoryError", NULL);
        return;
    }
    ALOGV("Java Binder %p: acquiring first ref on holder %p", obj, jbh);
    jbh->incStrong((void*)android_os_Binder_init);
    env->SetLongField(obj, gBinderOffsets.mObject, (jlong)jbh);
}

static void android_os_Binder_destroy(JNIEnv* env, jobject obj)
{
    JavaBBinderHolder* jbh = (JavaBBinderHolder*)
        env->GetLongField(obj, gBinderOffsets.mObject);
    if (jbh != NULL) {
        env->SetLongField(obj, gBinderOffsets.mObject, 0);
        ALOGV("Java Binder %p: removing ref on holder %p", obj, jbh);
        jbh->decStrong((void*)android_os_Binder_init);
    } else {
        // Encountering an uninitialized binder is harmless.  All it means is that
        // the Binder was only partially initialized when its finalizer ran and called
        // destroy().  The Binder could be partially initialized for several reasons.
        // For example, a Binder subclass constructor might have thrown an exception before
        // it could delegate to its superclass's constructor.  Consequently init() would
        // not have been called and the holder pointer would remain NULL.
        ALOGV("Java Binder %p: ignoring uninitialized binder", obj);
    }
}

// ----------------------------------------------------------------------------

static const JNINativeMethod gBinderMethods[] = {
     /* name, signature, funcPtr */
    { "getCallingPid", "()I", (void*)android_os_Binder_getCallingPid },
    { "getCallingUid", "()I", (void*)android_os_Binder_getCallingUid },
    { "clearCallingIdentity", "()J", (void*)android_os_Binder_clearCallingIdentity },
    { "restoreCallingIdentity", "(J)V", (void*)android_os_Binder_restoreCallingIdentity },
    { "setThreadStrictModePolicy", "(I)V", (void*)android_os_Binder_setThreadStrictModePolicy },
    { "getThreadStrictModePolicy", "()I", (void*)android_os_Binder_getThreadStrictModePolicy },
    { "flushPendingCommands", "()V", (void*)android_os_Binder_flushPendingCommands },
    { "init", "()V", (void*)android_os_Binder_init },
    { "destroy", "()V", (void*)android_os_Binder_destroy }
};

const char* const kBinderPathName = "android/os/Binder";

static int int_register_android_os_Binder(JNIEnv* env)
{
    jclass clazz;

    clazz = env->FindClass(kBinderPathName);
    LOG_FATAL_IF(clazz == NULL, "Unable to find class android.os.Binder");

    gBinderOffsets.mClass = (jclass) env->NewGlobalRef(clazz);
    gBinderOffsets.mExecTransact
        = env->GetMethodID(clazz, "execTransact", "(IJJI)Z");
    assert(gBinderOffsets.mExecTransact);

    gBinderOffsets.mObject
        = env->GetFieldID(clazz, "mObject", "J");
    assert(gBinderOffsets.mObject);

    return AndroidRuntime::registerNativeMethods(
        env, kBinderPathName,
        gBinderMethods, NELEM(gBinderMethods));
}

// ****************************************************************************
// ****************************************************************************
// ****************************************************************************

namespace android {

jint android_os_Debug_getLocalObjectCount(JNIEnv* env, jobject clazz)
{
    return gNumLocalRefs;
}

jint android_os_Debug_getProxyObjectCount(JNIEnv* env, jobject clazz)
{
    return gNumProxyRefs;
}

jint android_os_Debug_getDeathObjectCount(JNIEnv* env, jobject clazz)
{
    return gNumDeathRefs;
}

}

static int getprocname(pid_t pid, char *buf, size_t len) {
    char filename[32];
    FILE *f;

    snprintf(filename, sizeof(filename), "/proc/%d/cmdline", pid);
    f = fopen(filename, "r");
    if (!f) {
        *buf = '\0';
        return 1;
    }
    if (!fgets(buf, len, f)) {
        *buf = '\0';
        fclose(f);
        return 2;
    }
    fclose(f);
    return 0;
}

static bool push_eventlog_string(char** pos, const char* end, const char* str) {
    jint len = strlen(str);
    int space_needed = 1 + sizeof(len) + len;
    if (end - *pos < space_needed) {
        ALOGW("not enough space for string. remain=%" PRIdPTR "; needed=%d",
             end - *pos, space_needed);
        return false;
    }
    **pos = EVENT_TYPE_STRING;
    (*pos)++;
    memcpy(*pos, &len, sizeof(len));
    *pos += sizeof(len);
    memcpy(*pos, str, len);
    *pos += len;
    return true;
}

static bool push_eventlog_int(char** pos, const char* end, jint val) {
    int space_needed = 1 + sizeof(val);
    if (end - *pos < space_needed) {
        ALOGW("not enough space for int.  remain=%" PRIdPTR "; needed=%d",
             end - *pos, space_needed);
        return false;
    }
    **pos = EVENT_TYPE_INT;
    (*pos)++;
    memcpy(*pos, &val, sizeof(val));
    *pos += sizeof(val);
    return true;
}


// ****************************************************************************
// ****************************************************************************
// ****************************************************************************

int register_android_os_Binder(JNIEnv* env)
{
    if (int_register_android_os_Binder(env) < 0)
        return -1;
    //if (int_register_android_os_BinderInternal(env) < 0)
    //    return -1;
    //if (int_register_android_os_BinderProxy(env) < 0)
    //    return -1;

    jclass clazz;

    clazz = env->FindClass("android/util/Log");
    LOG_FATAL_IF(clazz == NULL, "Unable to find class android.util.Log");
    gLogOffsets.mClass = (jclass) env->NewGlobalRef(clazz);
    gLogOffsets.mLogE = env->GetStaticMethodID(
        clazz, "e", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/Throwable;)I");
    assert(gLogOffsets.mLogE);

    clazz = env->FindClass("android/os/ParcelFileDescriptor");
    LOG_FATAL_IF(clazz == NULL, "Unable to find class android.os.ParcelFileDescriptor");
    gParcelFileDescriptorOffsets.mClass = (jclass) env->NewGlobalRef(clazz);
    gParcelFileDescriptorOffsets.mConstructor
        = env->GetMethodID(clazz, "<init>", "(Ljava/io/FileDescriptor;)V");

    clazz = env->FindClass("android/os/StrictMode");
    LOG_FATAL_IF(clazz == NULL, "Unable to find class android.os.StrictMode");
    gStrictModeCallbackOffsets.mClass = (jclass) env->NewGlobalRef(clazz);
    gStrictModeCallbackOffsets.mCallback = env->GetStaticMethodID(
        clazz, "onBinderStrictModePolicyChange", "(I)V");
    LOG_FATAL_IF(gStrictModeCallbackOffsets.mCallback == NULL,
                 "Unable to find strict mode callback.");

    return 0;
}
