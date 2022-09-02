

#ifndef OPENGLCAMERA2_BYTEFLOWRENDERCONTEXT_H
#define OPENGLCAMERA2_BYTEFLOWRENDERCONTEXT_H


#include <cstdint>
#include <jni.h>
#include "ByteFlowRender.h"
#include "vec2.hpp"
#define GL_RENDER_TYPE   0
#define CL_RENDER_TYPE   1

using namespace glm;

class MyGLRenderContext
{
public:
	MyGLRenderContext(int renderType);

	~MyGLRenderContext();

	static void CreateRenderContext(JNIEnv *env, jobject instance, jint renderType);

	static void StoreRenderContext(JNIEnv *env, jobject instance, MyGLRenderContext *pContext);

	static void DeleteRenderContext(JNIEnv *env, jobject instance);

	static MyGLRenderContext* GetRenderContext(JNIEnv *env, jobject instance);

	int Init(int initType);

	int UnInit();

	void UpdateFrame(int format, uint8_t *pBuffer, int width, int height);

	void SetTransformMatrix(float translateX, float translateY, float scaleX, float scaleY, int degree, int mirror);

	void OnSurfaceCreated();

	void OnSurfaceChanged(int width, int height);

	void OnDrawFrame();

private:
	static jfieldID s_ContextHandle;

	ByteFlowRender *m_pByteFlowRender;

	vec2            m_ViewPort;
	TransformMatrix m_TransformMatrix;
};


#endif
