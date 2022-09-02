#include "LogUtil.h"
#include "glm/gtc/matrix_transform.hpp"
#include <unistd.h>
#include "MyGLRender.h"
#include "GLUtils.h"
#include "GLShaders.h"

static const float VERTICES_COORS[] =
        {
                -1.f, 1.f,
                -1.f, -1.f,
                1.f, 1.f,
                1.f, -1.f
        };

static const float TEXTURE_COORS[] =
        {
                0, 0,
                0, 1,
                1, 0,
                1, 1
        };

MyGLRender::MyGLRender() :
        m_Program(0),
        m_VertexShader(0),
        m_FragShader(0),
        m_YTextureId(0),
        m_UTextureId(0),
        m_VTextureId(0),
        m_ExtRgbaTextureId(0),
        m_VertexCoorHandle(0),
        m_TextureCoorHandle(0),
        m_YTextureHandle(0),
        m_UTextureHandle(0),
        m_VTextureHandle(0),
        m_MVPHandle(0),
        m_MVPMatrix(1.0f),
        m_PeriodicFrameIndex(0),
        m_FrameIndex(-1) {
    LOGCATE("MyGLRender::MyGLRender");
    m_IsProgramChanged = false;
    m_IsUpdateExtTexture = false;
    memset(&m_ExtRgbaImage, 0, sizeof(NativeImage));

}

MyGLRender::~MyGLRender() {
    LOGCATE("MyGLRender::~MyGLRender");
}

int MyGLRender::Init(int initType) {
    LOGCATE("MyGLRender::OnSurfaceCreated");
    return 0;
}

int MyGLRender::UnInit() {
    LOGCATE("MyGLRender::UnInit");
    NativeImageUtil::FreeNativeImage(&m_RenderFrame);
    NativeImageUtil::FreeNativeImage(&m_ExtRgbaImage);

    return 0;
}

void MyGLRender::UpdateFrame(NativeImage *pImage) {
    LOGCATE("MyGLRender::UpdateFrame");
    if (pImage == nullptr) return;
    if (pImage->width != m_RenderFrame.width || pImage->height != m_RenderFrame.height) {
        if (m_RenderFrame.ppPlane[0] != NULL) {
            NativeImageUtil::FreeNativeImage(&m_RenderFrame);
        }
        memset(&m_RenderFrame, 0, sizeof(NativeImage));
        m_RenderFrame.width = pImage->width;
        m_RenderFrame.height = pImage->height;
        m_RenderFrame.format = pImage->format;
        NativeImageUtil::AllocNativeImage(&m_RenderFrame);
    }

    NativeImageUtil::CopyNativeImage(pImage, &m_RenderFrame);
}

void MyGLRender::SetTransformMatrix(float translateX, float translateY, float scaleX, float scaleY,
                                     int degree, int mirror) {
    LOGCATE("MyGLRender::SetTransformMatrix translateX = %f, translateY = %f, scaleX = %f, scaleY = %f, degree = %d, mirror = %d",
            translateX, translateY, scaleX, scaleY, degree, mirror);

    m_TransformMatrix.degree = degree;
    m_TransformMatrix.mirror = mirror;
    m_IsProgramChanged = true;

}

bool MyGLRender::CreateTextures() {
    LOGCATE("MyGLRender::CreateTextures");
    GLsizei yWidth = static_cast<GLsizei>(m_RenderFrame.width);
    GLsizei yHeight = static_cast<GLsizei>(m_RenderFrame.height);

    glActiveTexture(GL_TEXTURE0);
    glGenTextures(1, &m_YTextureId);
    glBindTexture(GL_TEXTURE_2D, m_YTextureId);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, yWidth, yHeight, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE,
                 NULL);

    if (!m_YTextureId) {
        GLUtils::CheckGLError("MyGLRender::CreateTextures Create Y texture");
        return false;
    }

    GLsizei uWidth = static_cast<GLsizei>(m_RenderFrame.width / 2);
    GLsizei uHeight = yHeight / 2;

    glActiveTexture(GL_TEXTURE1);
    glGenTextures(1, &m_UTextureId);
    glBindTexture(GL_TEXTURE_2D, m_UTextureId);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, uWidth, uHeight, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE,
                 NULL);

    if (!m_UTextureId) {
        GLUtils::CheckGLError("MyGLRender::CreateTextures Create U texture");
        return false;
    }

    GLsizei vWidth = static_cast<GLsizei>(m_RenderFrame.width / 2);
    GLsizei vHeight = (GLsizei) yHeight / 2;

    glActiveTexture(GL_TEXTURE2);
    glGenTextures(1, &m_VTextureId);
    glBindTexture(GL_TEXTURE_2D, m_VTextureId);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, vWidth, vHeight, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE,
                 NULL);

    if (!m_VTextureId) {
        GLUtils::CheckGLError("MyGLRender::CreateTextures Create V texture");
        return false;
    }

    return true;
}

bool MyGLRender::UpdateTextures() {
    LOGCATE("MyGLRender::UpdateTextures");
    if (m_RenderFrame.ppPlane[0] == NULL) {
        return false;
    }

    if (!m_YTextureId && !m_UTextureId && !m_VTextureId && !CreateTextures()) {
        return false;
    }

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_YTextureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, (GLsizei) m_RenderFrame.width,
                 (GLsizei) m_RenderFrame.height, 0,
                 GL_LUMINANCE, GL_UNSIGNED_BYTE, m_RenderFrame.ppPlane[0]);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_UTextureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, (GLsizei) m_RenderFrame.width >> 1,
                 (GLsizei) m_RenderFrame.height >> 1, 0,
                 GL_LUMINANCE, GL_UNSIGNED_BYTE, m_RenderFrame.ppPlane[1]);

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, m_VTextureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, (GLsizei) m_RenderFrame.width >> 1,
                 (GLsizei) m_RenderFrame.height >> 1, 0,
                 GL_LUMINANCE, GL_UNSIGNED_BYTE, m_RenderFrame.ppPlane[2]);

    if (m_IsUpdateExtTexture && m_ExtRgbaImage.ppPlane[0]) {
        if (m_ExtRgbaTextureId) {
            glDeleteTextures(1, &m_ExtRgbaTextureId);
        }
        glActiveTexture(GL_TEXTURE3);
        glGenTextures(1, &m_ExtRgbaTextureId);
        glBindTexture(GL_TEXTURE_2D, m_ExtRgbaTextureId);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_ExtRgbaImage.width, m_ExtRgbaImage.height, 0,
                     GL_RGBA,
                     GL_UNSIGNED_BYTE, m_ExtRgbaImage.ppPlane[0]);
        m_IsUpdateExtTexture = false;
    }

    return true;
}

bool MyGLRender::DeleteTextures() {
    LOGCATE("MyGLRender::DeleteTextures");
    if (m_YTextureId) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, 0);
        glDeleteTextures(1, &m_YTextureId);
        m_YTextureId = 0;
    }

    if (m_UTextureId) {
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, 0);
        glDeleteTextures(1, &m_UTextureId);
        m_UTextureId = 0;
    }

    if (m_VTextureId) {
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, 0);
        glDeleteTextures(1, &m_VTextureId);
        m_VTextureId = 0;
    }

    if (m_ExtRgbaTextureId) {
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, 0);
        glDeleteTextures(1, &m_ExtRgbaTextureId);
        m_ExtRgbaTextureId = 0;
    }

    return true;
}

void MyGLRender::OnSurfaceCreated() {
    LOGCATE("MyGLRender::OnSurfaceCreated");
    int nMaxTextureSize = 0;
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &nMaxTextureSize);
    LOGCATE("MyGLRender::OnSurfaceCreated GL_MAX_TEXTURE_SIZE = %d", nMaxTextureSize);
    if (!CreateProgram(kVertexShader, kFragmentShader0)) {
        LOGCATE("MyGLRender::OnSurfaceCreated create program fail.");
    }
}

void MyGLRender::OnSurfaceChanged(int width, int height) {
    LOGCATE("MyGLRender::OnSurfaceChanged [w, h] = [%d, %d]", width, height);
    m_ViewportWidth = width;
    m_ViewportHeight = height;
    m_IsProgramChanged = true;
}

void MyGLRender::OnDrawFrame() {
    LOGCATE("MyGLRender::OnDrawFrame");
    glViewport(0, 0, m_ViewportWidth, m_ViewportHeight);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable(GL_CULL_FACE);

    if (!UpdateTextures() || !UseProgram()) {
        LOGCATE("MyGLRender::OnDrawFrame skip frame");
        return;
    }
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    m_PeriodicFrameIndex++;
    m_FrameIndex++;
    //glDrawArrays(GL_TRIANGLES, 0, m_FragCoors.nCoorsCount);
}

void MyGLRender::UpdateMVPMatrix(glm::mat4 &mat4Matrix, TransformMatrix &transformMatrix) {
    LOGCATE("MyGLRender::UpdateMVPMatrix");
    float fFactorX = 1.0f;
    float fFactorY = 1.0f;

    if (transformMatrix.mirror == 1) {
        fFactorX = -1.0f;
    } else if (transformMatrix.mirror == 2) {
        fFactorY = -1.0f;
    }

    float fRotate = MATH_PI * transformMatrix.degree * 1.0f / 180;
    if (transformMatrix.mirror == 0) {
        if (transformMatrix.degree == 270) {
            fRotate = MATH_PI * 0.5;
        } else if (transformMatrix.degree == 180) {
            fRotate = MATH_PI;
        } else if (transformMatrix.degree == 90) {
            fRotate = MATH_PI * 1.5;
        }
    } else if (transformMatrix.mirror == 1) {
        if (transformMatrix.degree == 90) {
            fRotate = MATH_PI * 0.5;
        } else if (transformMatrix.degree == 180) {
            fRotate = MATH_PI;
        } else if (transformMatrix.degree == 270) {
            fRotate = MATH_PI * 1.5;
        }
    }

    float ratio = (float) m_RenderFrame.width / m_RenderFrame.height;
    glm::mat4 Projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 1.0f);
    //glm::mat4 Projection = glm::frustum(-ratio, ratio, -1.0f, 1.0f, 4.0f, 100.0f);
    //glm::mat4 Projection = glm::perspective(45.0f,4.0f/3.0f,0.1f,100.f);
    glm::mat4 View = glm::lookAt(
            glm::vec3(0, 0, 1), // Camera is at (0,0,1), in World Space
            glm::vec3(0, 0, 0), // and looks at the origin
            glm::vec3(0, 1, 0) // Head is up (set to 0,-1,0 to look upside-down)
    );

    // Model matrix : an identity matrix (model will be at the origin)
    glm::mat4 Model = glm::mat4(1.0f);
    Model = glm::scale(Model, glm::vec3(fFactorX * transformMatrix.scaleX,
                                        fFactorY * transformMatrix.scaleY, 1.0f));
    Model = glm::rotate(Model, fRotate, glm::vec3(0.0f, 0.0f, 1.0f));
    Model = glm::translate(Model,
                           glm::vec3(transformMatrix.translateX, transformMatrix.translateY, 0.0f));

    LOGCATE("SetMVP real: rotate %d,%.2f,%0.5f,%0.5f,%0.5f,%0.5f,", transformMatrix.degree, fRotate,
            transformMatrix.translateX, transformMatrix.translateY,
            fFactorX * transformMatrix.scaleX, fFactorY * transformMatrix.scaleY);

    mat4Matrix =
            Projection * View * Model;// Remember, matrix multiplication is the other way around

}

GLuint MyGLRender::UseProgram() {
    LOGCATE("MyGLRender::UseProgram");
    if (!m_Program) {
        LOGCATE("MyGLRender::UseProgram Could not use program.");
        return 0;
    }

    if (m_IsProgramChanged) {
        glUseProgram(m_Program);
        GLUtils::CheckGLError("MyGLRender::UseProgram");

        glVertexAttribPointer(m_VertexCoorHandle, 2, GL_FLOAT, GL_FALSE, 2 * 4, VERTICES_COORS);
        glEnableVertexAttribArray(m_VertexCoorHandle);

        glUniform1i(m_YTextureHandle, 0);
        glUniform1i(m_UTextureHandle, 1);
        glUniform1i(m_VTextureHandle, 2);

        glVertexAttribPointer(m_TextureCoorHandle, 2, GL_FLOAT, GL_FALSE, 2 * 4, TEXTURE_COORS);
        glEnableVertexAttribArray(m_TextureCoorHandle);

        m_IsProgramChanged = false;
    }

    //
    TransformMatrix transformMatrix = m_TransformMatrix;
    UpdateMVPMatrix(m_MVPMatrix, transformMatrix);
    glUniformMatrix4fv(m_MVPHandle, 1, GL_FALSE, &m_MVPMatrix[0][0]);

    return m_Program;
}

int
MyGLRender::CreateProgram(const char *pVertexShaderSource, const char *pFragShaderSource) {
    m_Program = GLUtils::CreateProgram(pVertexShaderSource, pFragShaderSource, m_VertexShader,
                                       m_FragShader);
    if (!m_Program) {
        GLUtils::CheckGLError("Create Program");
        LOGCATE("MyGLRender::CreateProgram Could not create program.");
        return 0;
    }

    m_YTextureHandle = glGetUniformLocation(m_Program, "s_textureY");
    m_UTextureHandle = glGetUniformLocation(m_Program, "s_textureU");
    m_VTextureHandle = glGetUniformLocation(m_Program, "s_textureV");

    m_VertexCoorHandle = (GLuint) glGetAttribLocation(m_Program, "position");
    m_TextureCoorHandle = (GLuint) glGetAttribLocation(m_Program, "texcoord");
    m_MVPHandle = glGetUniformLocation(m_Program, "MVP");

    return m_Program;
}

