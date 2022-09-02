#include <jni.h>
#include <string>
#include "LogUtil.h"
#include "MyGLRenderContext.h"

#ifdef __cplusplus
extern "C" {
#endif


extern "C"
JNIEXPORT void JNICALL
Java_com_example_opengl_1beatingheart_MyNativeRender_native_1Init(JNIEnv *env, jobject thiz) {
    MyGLRenderContext::GetInstance();
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_opengl_1beatingheart_MyNativeRender_native_1UnInit(JNIEnv *env, jobject thiz) {
    MyGLRenderContext::DestroyInstance();
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_opengl_1beatingheart_MyNativeRender_native_1OnSurfaceCreated(JNIEnv *env,
                                                                          jobject thiz) {
    MyGLRenderContext::GetInstance()->OnSurfaceCreated();
}


extern "C"
JNIEXPORT void JNICALL
Java_com_example_opengl_1beatingheart_MyNativeRender_native_1OnSurfaceChanged(JNIEnv *env, jobject thiz,
                                                                          jint width, jint height) {
    MyGLRenderContext::GetInstance()->OnSurfaceChanged(width, height);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_opengl_1beatingheart_MyNativeRender_native_1OnDrawFrame(JNIEnv *env, jobject thiz) {
    MyGLRenderContext::GetInstance()->OnDrawFrame();
}


#ifdef __cplusplus
}
#endif
