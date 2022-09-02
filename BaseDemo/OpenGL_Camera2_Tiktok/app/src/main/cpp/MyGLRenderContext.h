

#ifndef OPENGLCAMERA2_BYTEFLOWRENDERCONTEXT_H
#define OPENGLCAMERA2_BYTEFLOWRENDERCONTEXT_H


#include <cstdint>
#include <jni.h>
#include "ByteFlowRender.h"
#include "vec2.hpp"
#include "GLExampleBase.h"

#define GL_RENDER_TYPE   0
#define CL_RENDER_TYPE   1

#define PARAM_TYPE_SET_SHADER_INDEX   201
#define PARAM_TYPE_SET_EXAMPLE        202

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

	void LoadFragShaderScript(int shaderIndex, char *pShaderStr, int strLen);

	void SetTransformMatrix(float translateX, float translateY, float scaleX, float scaleY, int degree, int mirror);

	void SetParamsInt(int paramType, int param);

	void OnSurfaceCreated();

	void OnSurfaceChanged(int width, int height);

	void OnDrawFrame();

private:
	void CreateExample(int exampleIndex);

	static jfieldID s_ContextHandle;

	ByteFlowRender *m_pByteFlowRender;

	GLExampleBase  *m_pCurGlFilter;
	GLExampleBase  *m_pBeforeGlFilter;
	volatile bool   m_bIsExampleMode;

	vec2            m_ViewPort;
	TransformMatrix m_TransformMatrix;
};


#endif //OPENGLCAMERA2_BYTEFLOWRENDERCONTEXT_H
