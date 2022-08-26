#ifndef _GL_SHADER_H_
#define _GL_SHADER_H_

static const char kVertexShader[] =
		"#version 100\n"
		"varying vec2 v_texcoord;\n"
		"attribute vec4 position;\n"
		"attribute vec2 texcoord;\n"
		"uniform mat4 MVP;\n"
		"void main() {\n"
		"    v_texcoord = texcoord;\n"
		"    gl_Position = MVP*position;\n"
		"}";

// Pixel shader, YUV420 to RGB conversion.
static const char kFragmentShader0[] =
		"#version 100\n \
    precision highp float; \
    varying vec2 v_texcoord;\
    uniform lowp sampler2D s_textureY;\
    uniform lowp sampler2D s_textureU;\
    uniform lowp sampler2D s_textureV;\
    void main() {\
        float y, u, v, r, g, b;\
        y = texture2D(s_textureY, v_texcoord).r;\
        u = texture2D(s_textureU, v_texcoord).r;\
        v = texture2D(s_textureV, v_texcoord).r;\
        u = u - 0.5;\
        v = v - 0.5;\
        r = y + 1.403 * v;\
        g = y - 0.344 * u - 0.714 * v;\
        b = y + 1.770 * u;\
        gl_FragColor = vec4(r, g, b, 1.0);\
    }";


#endif //_GL_SHADER_H_
