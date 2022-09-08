#include <jni.h>
#include <string>
#include "LogUtil.h"
#include "MyGLRenderContext.h"

#ifdef __cplusplus
extern "C" {
#endif

extern "C"
JNIEXPORT void JNICALL
Java_com_example_opengl_1scratchcard_MyNativeRender_native_1Init(JNIEnv *env, jobject thiz) {
    MyGLRenderContext::GetInstance();
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_opengl_1scratchcard_MyNativeRender_native_1UnInit(JNIEnv *env, jobject thiz) {
    MyGLRenderContext::DestroyInstance();
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_opengl_1scratchcard_MyNativeRender_native_1SetImageData
        (JNIEnv *env, jobject instance, jint format, jint width, jint height, jbyteArray imageData){
    int len = env->GetArrayLength (imageData);
    uint8_t* buf = new uint8_t[len];
    env->GetByteArrayRegion(imageData, 0, len, reinterpret_cast<jbyte*>(buf));
    MyGLRenderContext::GetInstance()->SetImageData(format, width, height, buf);
    delete[] buf;
    env->DeleteLocalRef(imageData);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_opengl_1scratchcard_MyNativeRender_native_1OnSurfaceCreated(JNIEnv *env,
                                                                             jobject thiz) {
    MyGLRenderContext::GetInstance()->OnSurfaceCreated();
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_opengl_1scratchcard_MyNativeRender_native_1OnSurfaceChanged(JNIEnv *env,
                                                                             jobject thiz,
                                                                             jint width,
                                                                             jint height) {
    MyGLRenderContext::GetInstance()->OnSurfaceChanged(width, height);
}


extern "C"
JNIEXPORT void JNICALL
Java_com_example_opengl_1scratchcard_MyNativeRender_native_1OnDrawFrame(JNIEnv *env, jobject thiz) {
    MyGLRenderContext::GetInstance()->OnDrawFrame();
}


extern "C"
JNIEXPORT void JNICALL
Java_com_example_opengl_1scratchcard_MyNativeRender_native_1UpdateTransformMatrix
(JNIEnv *env, jobject instance, jfloat rotateX, jfloat rotateY, jfloat scaleX, jfloat scaleY){
    MyGLRenderContext::GetInstance()->UpdateTransformMatrix(rotateX, rotateY, scaleX, scaleY);
}


extern "C"
JNIEXPORT void JNICALL
Java_com_example_opengl_1scratchcard_MyNativeRender_native_1SetParamsFloat(JNIEnv *env,
                                                                           jobject thiz,
                                                                           jfloat value0,
                                                                           jfloat value1) {
    MyGLRenderContext::GetInstance()->SetParamsFloat(value0, value1);
}


extern "C"
JNIEXPORT void JNICALL
Java_com_example_opengl_1scratchcard_MyNativeRender_native_1SetParamsInt
        (JNIEnv *env, jobject instance, jint paramType, jint value0, jint value1)
{
    MyGLRenderContext::GetInstance()->SetParamsInt(paramType, value0, value1);
}


#ifdef __cplusplus
}
#endif
