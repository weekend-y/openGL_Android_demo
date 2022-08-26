#ifndef NDK_OPENGLES_3_0_BLC_H
#define NDK_OPENGLES_3_0_BLC_H

#include <detail/type_mat.hpp>
#include <detail/type_mat4x4.hpp>
#include <vec3.hpp>
#include <vec2.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include "GLExampleBase.h"

using namespace glm;

#define BF_LOOP_COUNT 200
#define BF_BANNER_HEIGHT 5

class BluelineChallengeExample : public GLExampleBase
{
public:
	BluelineChallengeExample();

	virtual ~BluelineChallengeExample();

	virtual void LoadImage(NativeImage *pImage);

    virtual void LoadMultiImageWithIndex(int index, NativeImage *pImage);

	virtual void Init();
	virtual void Draw(int width, int height);

	virtual void Destroy();

	virtual void SetTransformMatrix(TransformMatrix &transformMatrix);

	void UpdateMVPMatrix(glm::mat4 &mat4Matrix, TransformMatrix &transformMatrix);

private:
	GLuint m_TextureId;
	GLint m_MVPMatLoc;
	GLuint m_VaoId;
	GLuint m_VboIds[3];
	NativeImage m_RenderImage;
	NativeImage m_SrcImage;
    glm::mat4 m_MVPMatrix;

	int m_frameIndex;
	int m_loopCount;

	int m_bannerNum;

};


#endif //NDK_OPENGLES_3_0_BLC_H
