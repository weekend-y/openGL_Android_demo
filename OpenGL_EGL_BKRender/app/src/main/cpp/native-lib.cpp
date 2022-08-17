#include <jni.h>
#include <string>
#include "LogUtil.h"
#include "EGLRender.h"
#include "MyGLRenderContext.h"

#ifdef __cplusplus
extern "C" {
#endif


extern "C"
JNIEXPORT void JNICALL
Java_com_example_opengl_1egl_1bkrender_MyNativeRender_native_1Init(JNIEnv *env, jobject thiz) {
    MyGLRenderContext::GetInstance();
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_opengl_1egl_1bkrender_MyNativeRender_native_1UnInit(JNIEnv *env, jobject thiz) {
    MyGLRenderContext::DestroyInstance();
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_opengl_1egl_1bkrender_MyNativeRender_native_1OnSurfaceCreated(JNIEnv *env,
                                                                                   jobject thiz) {
    MyGLRenderContext::GetInstance()->OnSurfaceCreated();
}


extern "C"
JNIEXPORT void JNICALL
Java_com_example_opengl_1egl_1bkrender_MyNativeRender_native_1OnSurfaceChanged(JNIEnv *env, jobject thiz,
                                                                                   jint width, jint height) {
    MyGLRenderContext::GetInstance()->OnSurfaceChanged(width, height);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_opengl_1egl_1bkrender_MyNativeRender_native_1OnDrawFrame(JNIEnv *env, jobject thiz) {
    MyGLRenderContext::GetInstance()->OnDrawFrame();
}



extern "C"
JNIEXPORT void JNICALL
Java_com_example_opengl_1egl_1bkrender_NativeEglRender_native_1EglRenderInit(JNIEnv *env,
                                                                             jobject thiz) {
    EGLRender::GetInstance()->Init();
}extern "C"
JNIEXPORT void JNICALL
Java_com_example_opengl_1egl_1bkrender_NativeEglRender_native_1EglRenderSetImageData(JNIEnv *env,
                                                                                     jobject thiz,
                                                                                     jbyteArray data,
                                                                                     jint width,
                                                                                     jint height) {
    int len = env->GetArrayLength (data);
    uint8_t* buf = new uint8_t[len];
    env->GetByteArrayRegion(data, 0, len, reinterpret_cast<jbyte*>(buf));
    EGLRender::GetInstance()->SetImageData(buf, width, height);
    delete[] buf;
    env->DeleteLocalRef(data);
}extern "C"
JNIEXPORT void JNICALL
Java_com_example_opengl_1egl_1bkrender_NativeEglRender_native_1EglRenderSetIntParams(JNIEnv *env,
                                                                                     jobject thiz,
                                                                                     jint param_type,
                                                                                     jint param) {
    EGLRender::GetInstance()->SetIntParams(param_type, param);
}extern "C"
JNIEXPORT void JNICALL
Java_com_example_opengl_1egl_1bkrender_NativeEglRender_native_1EglRenderDraw(JNIEnv *env,
                                                                             jobject thiz) {
    EGLRender::GetInstance()->Draw();
}extern "C"
JNIEXPORT void JNICALL
Java_com_example_opengl_1egl_1bkrender_NativeEglRender_native_1EglRenderUnInit(JNIEnv *env,
                                                                               jobject thiz) {
    EGLRender::GetInstance()->UnInit();
}

#ifdef __cplusplus
}
#endif
