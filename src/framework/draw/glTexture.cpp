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

        case GLTexture::RGBA8I:
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

        case GLTexture::RGBA8I:
            return GL_RGBA8_SNORM;
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
        case GLTexture::RGB10A2:
            return GL_UNSIGNED_BYTE;
        case GLTexture::RGB32F:
        case GLTexture::RGBA32F:
        case GLTexture::DEPTH32:
        case GLTexture::DEPTH24_STENCIL8:
            return GL_FLOAT;

        case GLTexture::RGBA8I:
            return GL_BYTE;
        default:
            assert(false);
    }
}

GLTexture::GLTexture(unsigned wi, unsigned hi, int format) : w(wi), h(hi)
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

GLTexture::GLTexture(Image i) : w(i.w), h(i.h)
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

Image GLTexture::asImage() {
    Image i;
    i.w = w;
    i.h = h;
    i.pixels = (uint8_t*) malloc(w * h * 4); //XXX free
    glBindTexture(GL_TEXTURE_2D, id);
    glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, i.pixels);
    return i;
}
