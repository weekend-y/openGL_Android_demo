

#ifndef NDK_OPENGLES_3_0_GLSAMPLEBASE_H
#define NDK_OPENGLES_3_0_GLSAMPLEBASE_H

#include "stdint.h"
#include <GLES3/gl3.h>
#include "ImageDef.h"
#include "ByteFlowDef.h"
#include "thread"

//For PI define
#define MATH_PI 3.1415926535897932384626433832802

#define SAMPLE_TYPE                             300
#define SAMPLE_TYPE_KEY_CONVEYOR_BELT           SAMPLE_TYPE
#define SAMPLE_TYPE_KEY_BLUE_LINE_CHALLENGE     SAMPLE_TYPE + 1

using namespace std;

class GLExampleBase
{
public:
	GLExampleBase()
	{
		m_ProgramObj = 0;
		m_VertexShader = 0;
		m_FragmentShader = 0;

		m_SurfaceWidth = 0;
		m_SurfaceHeight = 0;

	}

	virtual ~GLExampleBase()
	{

	}

	virtual void LoadImage(NativeImage *pImage)
	{};

	virtual void LoadMultiImageWithIndex(int index, NativeImage *pImage)
	{};

	virtual void SetTransformMatrix(TransformMatrix &transformMatrix)
	{}

	virtual void SetTouchLocation(float x, float y)
	{}

	virtual void SetGravityXY(float x, float y)
	{}

	virtual void Init() = 0;
	virtual void Draw(int width, int height) = 0;

	virtual void Destroy() = 0;

protected:
	GLuint m_VertexShader;
	GLuint m_FragmentShader;
	GLuint m_ProgramObj;
    mutex m_Lock;
	int m_SurfaceWidth;
	int m_SurfaceHeight;
	TransformMatrix m_TransformMatrix;
};


#endif //NDK_OPENGLES_3_0_GLSAMPLEBASE_H
