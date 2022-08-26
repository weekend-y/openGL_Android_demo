#include <jni.h>
#include <string>
#include "MyGLRenderContext.h"

#ifdef __cplusplus
extern "C" {
#endif

extern "C"
JNIEXPORT void JNICALL
Java_com_example_opengl_1camera2_1filter_MyNativeRender_native_1CreateContext(JNIEnv *env, jobject instance, jint renderType) {
    MyGLRenderContext::CreateRenderContext(env, instance, renderType);
}


extern "C"
JNIEXPORT void JNICALL
Java_com_example_opengl_1camera2_1filter_MyNativeRender_native_1DestroyContext(JNIEnv *env, jobject instance) {
    MyGLRenderContext::DeleteRenderContext(env, instance);
}


extern "C"
JNIEXPORT jint JNICALL
Java_com_example_opengl_1camera2_1filter_MyNativeRender_native_1Init(JNIEnv *env, jobject instance, jint iniType) {
    MyGLRenderContext *pContext = MyGLRenderContext::GetRenderContext(env, instance);
    if(pContext) return pContext->Init(iniType);
    return -1;
}


extern "C"
JNIEXPORT jint JNICALL
Java_com_example_opengl_1camera2_1filter_MyNativeRender_native_1UnInit(JNIEnv *env, jobject instance) {
    MyGLRenderContext *pContext = MyGLRenderContext::GetRenderContext(env, instance);
    if(pContext) return pContext->UnInit();
    return 0;
}


extern "C"
JNIEXPORT void JNICALL
Java_com_example_opengl_1camera2_1filter_MyNativeRender_native_1UpdateFrame(JNIEnv *env, jobject instance, jint format, jbyteArray bytes, jint width, jint height) {
    int len = env->GetArrayLength (bytes);
    unsigned char* buf = new unsigned char[len];
    env->GetByteArrayRegion(bytes, 0, len, reinterpret_cast<jbyte*>(buf));

    MyGLRenderContext *pContext = MyGLRenderContext::GetRenderContext(env, instance);
    if(pContext) pContext->UpdateFrame(format, buf, width, height);

    delete[] buf;
}


extern "C"
JNIEXPORT void JNICALL
Java_com_example_opengl_1camera2_1filter_MyNativeRender_native_1LoadFilterData(JNIEnv *env, jobject instance, jint index, jint format, jint width, jint height, jbyteArray imageData) {
    int len = env->GetArrayLength (imageData);
    uint8_t* buf = new uint8_t[len];
    env->GetByteArrayRegion(imageData, 0, len, reinterpret_cast<jbyte*>(buf));
    MyGLRenderContext *pContext = MyGLRenderContext::GetRenderContext(env, instance);
    if(pContext) pContext->LoadLutImageData(index, format, width, height, buf);
    delete[] buf;
    env->DeleteLocalRef(imageData);
}


extern "C"
JNIEXPORT void JNICALL
Java_com_example_opengl_1camera2_1filter_MyNativeRender_native_1LoadShaderScript(JNIEnv *env, jobject instance, jint shaderIndex, jstring script) {
    int length = env->GetStringUTFLength(script);
    const char* cStr = env->GetStringUTFChars(script, JNI_FALSE);
    char *buf = static_cast<char *>(malloc(length + 1));
    memcpy(buf, cStr, length + 1);
    MyGLRenderContext *pContext = MyGLRenderContext::GetRenderContext(env, instance);
    if(pContext) pContext->LoadFragShaderScript(shaderIndex, buf, length + 1);
    free(buf);
    env->ReleaseStringUTFChars(script, cStr);
}


extern "C"
JNIEXPORT void JNICALL
Java_com_example_opengl_1camera2_1filter_MyNativeRender_native_1SetTransformMatrix
        (JNIEnv *env, jobject instance, jfloat translateX, jfloat translateY, jfloat scaleX, jfloat scaleY, jint degree, jint mirror){
    MyGLRenderContext *pContext = MyGLRenderContext::GetRenderContext(env, instance);
    if(pContext) pContext->SetTransformMatrix(translateX, translateY, scaleX, scaleY, degree, mirror);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_opengl_1camera2_1filter_MyNativeRender_native_1OnSurfaceCreated(JNIEnv *env, jobject instance) {
    MyGLRenderContext *pContext = MyGLRenderContext::GetRenderContext(env, instance);
    if (pContext) pContext->OnSurfaceCreated();
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_opengl_1camera2_1filter_MyNativeRender_native_1OnSurfaceChanged(JNIEnv *env, jobject instance, jint width, jint height) {
    MyGLRenderContext *pContext = MyGLRenderContext::GetRenderContext(env, instance);
    if (pContext) pContext->OnSurfaceChanged(width, height);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_opengl_1camera2_1filter_MyNativeRender_native_1OnDrawFrame(JNIEnv *env, jobject instance) {
    MyGLRenderContext *pContext = MyGLRenderContext::GetRenderContext(env, instance);
    if (pContext) pContext->OnDrawFrame();
}


#ifdef __cplusplus
}
#endif
