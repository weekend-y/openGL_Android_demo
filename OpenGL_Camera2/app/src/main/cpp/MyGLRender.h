
#ifndef OPENGLCAMERA2_MYGLRENDER_H
#define OPENGLCAMERA2_MYGLRENDER_H

#include "glm/detail/type_mat.hpp"
#include "glm/detail/type_mat4x4.hpp"
#include <GLES3/gl3.h>
#include <vector>
#include "ImageDef.h"
#include "ByteFlowRender.h"

#define MAX_FRAME_NUM  30
#define SKIP_FRAME_NUM 5

class MyGLRender : public ByteFlowRender
{
public:
	MyGLRender();
	virtual ~MyGLRender();

	virtual int Init(int initType);

	virtual int UnInit();

	virtual void UpdateFrame(NativeImage *pImage);

	virtual void SetTransformMatrix(float translateX, float translateY, float scaleX, float scaleY, int degree, int mirror);

	virtual bool CreateTextures();

	virtual bool UpdateTextures();

	virtual bool DeleteTextures();

	virtual void OnSurfaceCreated();

	virtual void OnSurfaceChanged(int width, int height);

	virtual void OnDrawFrame();

	int CreateProgram(const char *pVertexShaderSource, const char *pFragShaderSource);

	void UpdateMVPMatrix(glm::mat4 &mat4Matrix, TransformMatrix &transformMatrix);

	GLuint UseProgram();

private:
	glm::mat4 m_MVPMatrix;

	GLuint m_YTextureId;
	GLuint m_UTextureId;
	GLuint m_VTextureId;
	GLuint m_ExtRgbaTextureId;

	GLuint m_Program;
	GLuint m_VertexShader;
	GLuint m_FragShader;

	GLuint m_VertexCoorHandle;
	GLuint m_TextureCoorHandle;

	GLint m_YTextureHandle;
	GLint m_UTextureHandle;
	GLint m_VTextureHandle;

	GLint m_MVPHandle;

	int m_PeriodicFrameIndex;
	int m_FrameIndex;
	NativeImage m_ExtRgbaImage;
	SyncLock m_SynLock;

	volatile bool m_IsUpdateExtTexture;
};


#endif //
