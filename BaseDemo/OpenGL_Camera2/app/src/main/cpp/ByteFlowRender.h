
#ifndef OPENGLCAMERA2_BYTEFLOWRENDER_H
#define OPENGLCAMERA2_BYTEFLOWRENDER_H

#include "SyncLock.h"
#include "ByteFlowDef.h"
#include "ImageDef.h"

class ByteFlowRender
{
public:
	ByteFlowRender():
	m_ViewportHeight(0),
	m_ViewportWidth(0),
	m_IsProgramChanged(false),
	m_IsTextureDirty(false)
	{
		memset(&m_RenderFrame, 0, sizeof(NativeImage));
	}

	virtual ~ByteFlowRender()
	{

	}

	virtual int Init(int initType) = 0;
	virtual int UnInit() = 0;

    virtual void UpdateFrame(NativeImage *pImage) = 0;

    virtual void SetTransformMatrix(float translateX, float translateY, float scaleX, float scaleY, int degree, int mirror) = 0;

    virtual bool CreateTextures() = 0;

    virtual bool UpdateTextures() = 0;

    virtual bool DeleteTextures() = 0;

    virtual void OnSurfaceCreated() = 0;

    virtual void OnSurfaceChanged(int width, int height) = 0;

    virtual void OnDrawFrame() = 0;


protected:
	volatile bool   m_IsProgramChanged;
	size_t          m_ViewportWidth;
	size_t          m_ViewportHeight;
	NativeImage     m_RenderFrame;
	TransformMatrix m_TransformMatrix;

	volatile bool   m_IsTextureDirty;
};

#endif //OPENGLCAMERA2_BYTEFLOWRENDER_H
