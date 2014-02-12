#include "glTexture.hpp"

#include<assert.h>

static int getGLFormat(int format)
{
    switch(format)
    {
        case GLTexture::DEPTH24_STENCIL8:
            return GL_DEPTH_STENCIL;
        case GLTexture::DEPTH32:
            return GL_DEPTH_COMPONENT;
        case GLTexture::RGBA8:
            return GL_RGBA;
        case GLTexture::RGB8:
            return GL_RGB;
        case GLTexture::RGB10A2:
            return GL_RGB;
        case GLTexture::RGB32F:
            return GL_RGB;
        case GLTexture::RGBA32F:
            return GL_RGBA;
        default:
            assert(false);
    }
}

static int getGLInternalFormat(int format)
{
    switch(format)
    {
        case GLTexture::RGBA8:
            return GL_RGBA8;
        case GLTexture::RGB8:
            return GL_RGB8;
        case GLTexture::RGB10A2:
            return GL_RGB10_A2;
        case GLTexture::RGB32F:
            return GL_RGB32F;
        case GLTexture::RGBA32F:
            return GL_RGBA32F;
        case GLTexture::DEPTH32:
            return GL_DEPTH_COMPONENT32;
        case GLTexture::DEPTH24_STENCIL8:
            return GL_DEPTH24_STENCIL8;
        default:
            assert(false);
    }
}

static int getGLType(int format)
{
    switch(format)
    {
        case GLTexture::RGBA8:
        case GLTexture::RGB8:
            return GL_UNSIGNED_BYTE;
        case GLTexture::RGB10A2:
            return GL_UNSIGNED_BYTE;
        case GLTexture::RGB32F:
        case GLTexture::RGBA32F:
        case GLTexture::DEPTH32:
        case GLTexture::DEPTH24_STENCIL8:
            return GL_FLOAT;
        default:
            assert(false);
    }
}

GLTexture::GLTexture(unsigned w, unsigned h, int format)
{
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexImage2D(GL_TEXTURE_2D, 0, getGLInternalFormat(format), w, h, 0, 
            getGLFormat(format), getGLType(format), 0);
}

GLTexture::GLTexture(Image &i)
{
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexImage2D(GL_TEXTURE_2D, 0, getGLInternalFormat(i.format), 
            i.w, i.h, 0, getGLFormat(i.format), 
            getGLType(i.format), i.pixels);
    //TODO: getGLFormat expects GLTexture::Format, not Image::Format
}
