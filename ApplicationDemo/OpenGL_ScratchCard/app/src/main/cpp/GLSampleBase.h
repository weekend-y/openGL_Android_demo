#ifndef NDK_OPENGLES_3_0_GLSAMPLEBASE_H
#define NDK_OPENGLES_3_0_GLSAMPLEBASE_H

#include "stdint.h"
#include <GLES3/gl3.h>
#include "ImageDef.h"
#include "MyLock.h"

//For PI define
#define MATH_PI 3.1415926535897932384626433832802

#define SAMPLE_TYPE                             200
#define SAMPLE_TYPE_GUAGUALE                	SAMPLE_TYPE + 0
#define SAMPLE_TYPE_SHOUXIEBAN              	SAMPLE_TYPE + 1


class GLSampleBase
{
public:
	GLSampleBase()
	{
		m_ProgramObj = 0;
		m_VertexShader = 0;
		m_FragmentShader = 0;

		m_SurfaceWidth = 0;
		m_SurfaceHeight = 0;

	}

	virtual ~GLSampleBase()
	{

	}

	virtual void LoadImage(NativeImage *pImage)
	{};

	virtual void LoadMultiImageWithIndex(int index, NativeImage *pImage)
	{};

	virtual void LoadShortArrData(short *const pShortArr, int arrSize)
	{}

	virtual void UpdateTransformMatrix(float rotateX, float rotateY, float scaleX, float scaleY)
	{}

	virtual void SetTouchLocation(float x, float y)
	{}

	virtual void SetGravityXY(float x, float y)
	{}

	virtual void Init() = 0;
	virtual void Draw(int screenW, int screenH) = 0;

	virtual void Destroy() = 0;

protected:
	GLuint m_VertexShader;
	GLuint m_FragmentShader;
	GLuint m_ProgramObj;
	MySyncLock m_Lock;
	int m_SurfaceWidth;
	int m_SurfaceHeight;
};


#endif //NDK_OPENGLES_3_0_GLSAMPLEBASE_H
