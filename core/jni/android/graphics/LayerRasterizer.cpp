#include "SkLayerRasterizer.h"
#include <jni.h>

class SkLayerRasterizerGlue {
public:
    static jlong create(JNIEnv* env, jobject) {
        return reinterpret_cast<jlong>(new SkLayerRasterizer::Builder());
    }

    static void addLayer(JNIEnv* env, jobject, jlong layerHandle, jlong paintHandle, jfloat dx, jfloat dy) {
        SkLayerRasterizer::Builder* builder = reinterpret_cast<SkLayerRasterizer::Builder *>(layerHandle);
        const SkPaint* paint = reinterpret_cast<SkPaint *>(paintHandle);
        SkASSERT(builder);
        SkASSERT(paint);
        builder->addLayer(*paint, SkFloatToScalar(dx), SkFloatToScalar(dy));
    }
};

/////////////////////////////////////////////////////////////////////////////////////////

#include <android_runtime/AndroidRuntime.h>

static JNINativeMethod gLayerRasterizerMethods[] = {
    { "nativeConstructor",  "()J",      (void*)SkLayerRasterizerGlue::create    },
    { "nativeAddLayer",     "(JJFF)V",  (void*)SkLayerRasterizerGlue::addLayer  }
};

int register_android_graphics_LayerRasterizer(JNIEnv* env)
{
    return android::AndroidRuntime::registerNativeMethods(env,
                                                       "android/graphics/LayerRasterizer",
                                                       gLayerRasterizerMethods,
                                                       SK_ARRAY_COUNT(gLayerRasterizerMethods));
}
