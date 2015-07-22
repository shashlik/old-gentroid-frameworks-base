/*
 * Copyright (C) 2011 The Android Open Source Project
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

#define LOG_TAG "InputEventReceiver"

//#define LOG_NDEBUG 0

// Log debug messages about the dispatch cycle.
#define DEBUG_DISPATCH_CYCLE 0


#include "JNIHelp.h"

#include <android_runtime/AndroidRuntime.h>
#include <utils/Log.h>
#include <utils/Looper.h>
#include <utils/Vector.h>
#include <utils/threads.h>
#include "android_os_MessageQueue.h"
#include "android_view_KeyEvent.h"
#include "android_view_MotionEvent.h"

#include <ScopedLocalRef.h>

namespace android {

static struct {
    jclass clazz;

    jmethodID dispatchInputEvent;
    jmethodID dispatchBatchedInputEventPending;
} gInputEventReceiverClassInfo;




static jlong nativeInit(JNIEnv* env, jclass clazz, jobject receiverWeak,
        jobject inputChannelObj, jobject messageQueueObj) {
    return 0;
}

static void nativeDispose(JNIEnv* env, jclass clazz, jlong receiverPtr) {
}

static void nativeFinishInputEvent(JNIEnv* env, jclass clazz, jlong receiverPtr,
        jint seq, jboolean handled) {
}

static jboolean nativeConsumeBatchedInputEvents(JNIEnv* env, jclass clazz, jlong receiverPtr,
        jlong frameTimeNanos) {
        return JNI_FALSE;
}


static JNINativeMethod gMethods[] = {
    /* name, signature, funcPtr */
    { "nativeInit",
            "(Ljava/lang/ref/WeakReference;Landroid/view/InputChannel;Landroid/os/MessageQueue;)J",
            (void*)nativeInit },
    { "nativeDispose", "(J)V",
            (void*)nativeDispose },
    { "nativeFinishInputEvent", "(JIZ)V",
            (void*)nativeFinishInputEvent },
    { "nativeConsumeBatchedInputEvents", "(JJ)Z",
            (void*)nativeConsumeBatchedInputEvents },
};

#define FIND_CLASS(var, className) \
        var = env->FindClass(className); \
        LOG_FATAL_IF(! var, "Unable to find class " className); \
        var = jclass(env->NewGlobalRef(var));

#define GET_METHOD_ID(var, clazz, methodName, methodDescriptor) \
        var = env->GetMethodID(clazz, methodName, methodDescriptor); \
        LOG_FATAL_IF(! var, "Unable to find method " methodName);

int register_android_view_InputEventReceiver(JNIEnv* env) {
    int res = jniRegisterNativeMethods(env, "android/view/InputEventReceiver",
            gMethods, NELEM(gMethods));
    LOG_FATAL_IF(res < 0, "Unable to register native methods.");

    FIND_CLASS(gInputEventReceiverClassInfo.clazz, "android/view/InputEventReceiver");

    GET_METHOD_ID(gInputEventReceiverClassInfo.dispatchInputEvent,
            gInputEventReceiverClassInfo.clazz,
            "dispatchInputEvent", "(ILandroid/view/InputEvent;)V");
    GET_METHOD_ID(gInputEventReceiverClassInfo.dispatchBatchedInputEventPending,
            gInputEventReceiverClassInfo.clazz,
            "dispatchBatchedInputEventPending", "()V");
    return 0;
}

} // namespace android
