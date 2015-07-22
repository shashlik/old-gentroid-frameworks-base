/*
 * Copyright (C) 2008 The Android Open Source Project
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

#include "jni.h"
#include "GraphicsJNI.h"
#include <android_runtime/AndroidRuntime.h>

#include "SkCanvas.h"
#include "SkPicture.h"
#include "SkPictureRecorder.h"
#include "SkStream.h"
#include "SkTemplates.h"
#include "CreateJavaOutputStreamAdaptor.h"

namespace android {

class SkPictureGlue {
public:
    static jlong newPicture(JNIEnv* env, jobject, jlong srcHandle) {
        const SkPicture* src = reinterpret_cast<SkPicture*>(srcHandle);
        if (src) {
            return reinterpret_cast<jlong>(src->clone());
        } else {
            return reinterpret_cast<jlong>(new SkPicture());
        }
    }

	static jlong createRec(JNIEnv* env, jobject) {
    	return reinterpret_cast<jlong>(new SkPictureRecorder);
    }

    static jlong deserialize(JNIEnv* env, jobject, jobject jstream,
                             jbyteArray jstorage) {
        SkPicture* picture = NULL;
        SkStream* strm = CreateJavaInputStreamAdaptor(env, jstream, jstorage);
        if (strm) {
            picture = SkPicture::CreateFromStream(strm);
            delete strm;
        }
        return reinterpret_cast<jlong>(picture);
    }

    static void killPicture(JNIEnv* env, jobject, jlong pictureHandle) {
        SkPicture* picture = reinterpret_cast<SkPicture*>(pictureHandle);
        SkASSERT(picture);
        picture->unref();
    }
	
	static void killRec(JNIEnv* env, jobject, jlong recHandle) {
        SkPictureRecorder* rec = reinterpret_cast<SkPictureRecorder*>(recHandle);
        SkASSERT(rec);
        delete rec;
    }

    static void draw(JNIEnv* env, jobject, jlong canvasHandle,
                            jlong pictureHandle) {
        SkCanvas* canvas = reinterpret_cast<SkCanvas*>(canvasHandle);
        SkPicture* picture = reinterpret_cast<SkPicture*>(pictureHandle);
        SkASSERT(canvas);
        SkASSERT(picture);
        picture->draw(canvas);
    }

    static jboolean serialize(JNIEnv* env, jobject, jlong pictureHandle,
                          jobject jstream, jbyteArray jstorage) {
        SkPicture* picture = reinterpret_cast<SkPicture*>(pictureHandle);
        SkWStream* strm = CreateJavaOutputStreamAdaptor(env, jstream, jstorage);
        
        if (NULL != strm) {
            picture->serialize(strm);
            delete strm;
            return JNI_TRUE;
        }
        return JNI_FALSE;
    }

    static jint getWidth(JNIEnv* env, jobject jpic) {
        NPE_CHECK_RETURN_ZERO(env, jpic);
        int width = GraphicsJNI::getNativePicture(env, jpic)->width();
        return static_cast<jint>(width);
    }

    static jint getHeight(JNIEnv* env, jobject jpic) {
        NPE_CHECK_RETURN_ZERO(env, jpic);
        int height = GraphicsJNI::getNativePicture(env, jpic)->height();
        return static_cast<jint>(height);
    }

    static jlong beginRecording(JNIEnv* env, jobject, jlong recHandle,
                                    jint w, jint h) {
        SkPictureRecorder* rec = reinterpret_cast<SkPictureRecorder*>(recHandle);
		SkASSERT(rec);
        // beginRecording does not ref its return value, it just returns it.
        SkCanvas* canvas = rec->beginRecording(w, h);
        // the java side will wrap this guy in a Canvas.java, which will call
        // unref in its finalizer, so we have to ref it here, so that both that
        // Canvas.java and our picture can both be owners
        canvas->ref();
        return reinterpret_cast<jlong>(canvas);
    }

    static void endRecording(JNIEnv* env, jobject, jlong recHandle) {
        SkPictureRecorder* rec = reinterpret_cast<SkPictureRecorder*>(recHandle);
		SkASSERT(rec);
        rec->endRecording();
    }
};
/*
static JNINativeMethod gPictureMethods[] = {
    {"getWidth", "()I", (void*) SkPictureGlue::getWidth},
    {"getHeight", "()I", (void*) SkPictureGlue::getHeight},
    {"nativeConstructor", "(J)J", (void*) SkPictureGlue::newPicture},
	{"nativeCreateRec", "()J", (void*) SkPictureGlue::createRec},
    {"nativeCreateFromStream", "(Ljava/io/InputStream;[B)J", (void*)SkPictureGlue::deserialize},
    {"nativeBeginRecording", "(JII)J", (void*) SkPictureGlue::beginRecording},
    {"nativeEndRecording", "(J)V", (void*) SkPictureGlue::endRecording},
    {"nativeDraw", "(JJ)V", (void*) SkPictureGlue::draw},
    {"nativeWriteToStream", "(JLjava/io/OutputStream;[B)Z", (void*)SkPictureGlue::serialize},
    {"nativeDestructor","(J)V", (void*) SkPictureGlue::killPicture},
	{"nativeKillRec","(J)V", (void*) SkPictureGlue::killRec}
};
*/
#include <android_runtime/AndroidRuntime.h>

#define REG(env, name, array) \
    result = android::AndroidRuntime::registerNativeMethods(env, name, array, \
    SK_ARRAY_COUNT(array));  \
    if (result < 0) return result

int register_android_graphics_Picture(JNIEnv* env) {
    int result;

    //REG(env, "android/graphics/Picture", gPictureMethods);

    return result;
}

}


