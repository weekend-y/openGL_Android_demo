#ifndef NDK_OPENGLES_3_0_FBOBLITSAMPLE_H
#define NDK_OPENGLES_3_0_FBOBLITSAMPLE_H

#include <detail/type_mat.hpp>
#include <detail/type_mat4x4.hpp>
#include "GLSampleBase.h"
#define ATTACHMENT_NUM 4
class FBOBlitSample : public GLSampleBase
{
public:
	FBOBlitSample();

	virtual ~FBOBlitSample();

	virtual void LoadImage(NativeImage *pImage);

	virtual void Init();
	virtual void Draw(int screenW, int screenH);

	virtual void Destroy();

	virtual void UpdateTransformMatrix(float rotateX, float rotateY, float scaleX, float scaleY);

	void UpdateMVPMatrix(glm::mat4 &mvpMatrix, int angleX, int angleY, float ratio);

    bool InitFBO();

    void BlitTextures();

private:
	GLuint m_TextureId;
    GLuint m_AttachTexIds[ATTACHMENT_NUM];
    GLuint m_FBO;
	GLint m_SamplerLoc;
	GLint m_MVPMatLoc;
	GLuint m_VaoId;
	GLuint m_VboIds[3];
	NativeImage m_RenderImage;
	glm::mat4 m_MVPMatrix;

	int m_AngleX;
	int m_AngleY;
	float m_ScaleX;
	float m_ScaleY;

};


#endif //NDK_OPENGLES_3_0_FBOBLITSAMPLE_H
