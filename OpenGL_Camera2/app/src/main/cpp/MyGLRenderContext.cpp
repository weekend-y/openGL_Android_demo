
#include "MyGLRender.h"
#include "LogUtil.h"
#include "MyGLRenderContext.h"

jfieldID MyGLRenderContext::s_ContextHandle = 0L;

MyGLRenderContext::MyGLRenderContext(int renderType) : m_pByteFlowRender(nullptr)
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

	m_pByteFlowRender->UpdateFrame(&nativeImage);
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
    m_pByteFlowRender->OnDrawFrame();
}

