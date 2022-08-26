
#include "MyGLRender.h"
#include "LogUtil.h"
#include "MyGLRenderContext.h"
#include "ConveyorBeltExample_1.h"
#include "BluelineChallengeExample.h"

jfieldID MyGLRenderContext::s_ContextHandle = 0L;

MyGLRenderContext::MyGLRenderContext(int renderType) :
		m_pByteFlowRender(nullptr), m_pCurGlFilter(nullptr), m_pBeforeGlFilter(nullptr), m_bIsExampleMode(
		false)
{
	switch (renderType)
	{
		case GL_RENDER_TYPE:
			m_pByteFlowRender = new MyGLRender();
			break;
		case CL_RENDER_TYPE:
			break;
		default:
			m_pByteFlowRender = new MyGLRender();
	}

}

MyGLRenderContext::~MyGLRenderContext()
{
	if (m_pByteFlowRender != NULL)
	{
		delete m_pByteFlowRender;
		m_pByteFlowRender = NULL;
	}

}

void MyGLRenderContext::CreateRenderContext(JNIEnv *env, jobject instance, jint renderType)
{
	LOGCATE("MyGLRenderContext::CreateRenderContext renderType = %d", renderType);
	MyGLRenderContext *pContext = new MyGLRenderContext(renderType);
	StoreRenderContext(env, instance, pContext);
}

void MyGLRenderContext::StoreRenderContext(JNIEnv *env, jobject instance, MyGLRenderContext *pContext)
{
	LOGCATE("MyGLRenderContext::StoreRenderContext");
	jclass cls = env->GetObjectClass(instance);
	if (cls == NULL)
	{
		LOGCATE("MyGLRenderContext::StoreRenderContext cls == NULL");
		return;
	}

	s_ContextHandle = env->GetFieldID(cls, "mNativeContextHandle", "J");
	if (s_ContextHandle == NULL)
	{
		LOGCATE("MyGLRenderContext::StoreRenderContext s_ContextHandle == NULL");
		return;
	}

	env->SetLongField(instance, s_ContextHandle, reinterpret_cast<jlong>(pContext));

}


void MyGLRenderContext::DeleteRenderContext(JNIEnv *env, jobject instance)
{
	LOGCATE("MyGLRenderContext::DeleteRenderContext");
	if (s_ContextHandle == NULL)
	{
		LOGCATE("MyGLRenderContext::DeleteRenderContext Could not find render context.");
		return;
	}

	MyGLRenderContext *pContext = reinterpret_cast<MyGLRenderContext *>(env->GetLongField(
			instance, s_ContextHandle));
	if (pContext)
	{
		delete pContext;
	}
	env->SetLongField(instance, s_ContextHandle, 0L);
}

MyGLRenderContext *MyGLRenderContext::GetRenderContext(JNIEnv *env, jobject instance)
{
	LOGCATE("MyGLRenderContext::GetRenderContext");

	if (s_ContextHandle == NULL)
	{
		LOGCATE("MyGLRenderContext::GetRenderContext Could not find render context.");
		return NULL;
	}

	MyGLRenderContext *pContext = reinterpret_cast<MyGLRenderContext *>(env->GetLongField(
			instance, s_ContextHandle));
	return pContext;
}

int MyGLRenderContext::Init(int initType)
{
	return m_pByteFlowRender->Init(initType);
}

int MyGLRenderContext::UnInit()
{
	if(m_pCurGlFilter) {
		delete m_pCurGlFilter;
		m_pCurGlFilter = nullptr;
	}

	if(m_pBeforeGlFilter) {
		delete m_pBeforeGlFilter;
		m_pBeforeGlFilter = nullptr;
	}
	return m_pByteFlowRender->UnInit();
}

void MyGLRenderContext::UpdateFrame(int format, uint8_t *pBuffer, int width, int height)
{
	LOGCATE("MyGLRenderContext::UpdateFrame format=%d, width=%d, height=%d, pData=%p",
			format, width, height, pBuffer);
	NativeImage nativeImage;
	nativeImage.format = format;
	nativeImage.width = width;
	nativeImage.height = height;
	nativeImage.ppPlane[0] = pBuffer;

	switch (format)
	{
		case IMAGE_FORMAT_NV12:
		case IMAGE_FORMAT_NV21:
			nativeImage.ppPlane[1] = nativeImage.ppPlane[0] + width * height;
			break;
		case IMAGE_FORMAT_I420:
			nativeImage.ppPlane[1] = nativeImage.ppPlane[0] + width * height;
			nativeImage.ppPlane[2] = nativeImage.ppPlane[1] + width * height / 4;
			break;
		default:
			break;
	}

	if(m_bIsExampleMode && m_pCurGlFilter) {
		m_pCurGlFilter->LoadImage(&nativeImage);
	} else {
		m_pByteFlowRender->UpdateFrame(&nativeImage);
	}
}

void MyGLRenderContext::SetTransformMatrix(float translateX, float translateY, float scaleX, float scaleY, int degree, int mirror)
{
	m_pByteFlowRender->SetTransformMatrix(translateX, translateY, scaleX, scaleY, degree, mirror);

	m_TransformMatrix.translateX = translateX;
	m_TransformMatrix.translateY = translateY;
	m_TransformMatrix.scaleX = scaleX;
	m_TransformMatrix.scaleY = scaleY;
	m_TransformMatrix.degree = degree;
	m_TransformMatrix.mirror = mirror;
}

void MyGLRenderContext::SetParamsInt(int paramType, int param)
{
	LOGCATE("MyGLRenderContext::SetParamsInt paramType = %d, param = %d", paramType, param);
	switch (paramType)
	{
//		case PARAM_TYPE_SET_SHADER_INDEX:
//			m_bIsExampleMode = false;
//			m_pByteFlowRender->SetShaderIndex(param);
//			break;
		case PARAM_TYPE_SET_EXAMPLE:
			CreateExample(param);
			break;
		default:
			break;
	}

}

void MyGLRenderContext::OnSurfaceCreated()
{
	m_pByteFlowRender->OnSurfaceCreated();
}

void MyGLRenderContext::OnSurfaceChanged(int width, int height)
{
	m_ViewPort = vec2(width, height);
	m_pByteFlowRender->OnSurfaceChanged(width, height);
}

void MyGLRenderContext::OnDrawFrame()
{
	if(m_bIsExampleMode) {
		if(m_pBeforeGlFilter) {
			m_pBeforeGlFilter->Destroy();
			delete m_pBeforeGlFilter;
			m_pBeforeGlFilter = nullptr;
		}

		if(m_pCurGlFilter) {
			m_pCurGlFilter->Init();
			m_pCurGlFilter->SetTransformMatrix(m_TransformMatrix);
			m_pCurGlFilter->Draw(m_ViewPort.x, m_ViewPort.y);
		}

	}
	else {
		m_pByteFlowRender->OnDrawFrame();
	}
}

void MyGLRenderContext::LoadFragShaderScript(int shaderIndex, char *pShaderStr, int strLen)
{
	LOGCATE("MyGLRenderContext::LoadFragShaderScript shaderIndex = %d, pShaderStr = %s, strLen = %d", shaderIndex, pShaderStr, strLen);

    if(m_bIsExampleMode) {
        m_bIsExampleMode = false;
		if(m_pBeforeGlFilter) {
			m_pBeforeGlFilter->Destroy();
			delete m_pBeforeGlFilter;
			m_pBeforeGlFilter = nullptr;
		}

		if(m_pCurGlFilter) {
			m_pCurGlFilter->Destroy();
			delete m_pCurGlFilter;
			m_pCurGlFilter = nullptr;
		}
    }
	m_pByteFlowRender->LoadFragShaderScript(shaderIndex, pShaderStr, strLen);

}

void MyGLRenderContext::CreateExample(int exampleIndex) {
	LOGCATE("MyGLRenderContext:CreateExample exampleIndex=%d", exampleIndex);
	m_pBeforeGlFilter = m_pCurGlFilter;
	switch (exampleIndex) {
		case SAMPLE_TYPE_KEY_CONVEYOR_BELT:
			m_pCurGlFilter = new ConveyorBeltExample_1();
			break;
		case SAMPLE_TYPE_KEY_BLUE_LINE_CHALLENGE:
			m_pCurGlFilter = new BluelineChallengeExample();
			break;
		default:
			m_pCurGlFilter = nullptr;
			break;
	}

	if(m_pCurGlFilter != nullptr) m_bIsExampleMode = true;
}



