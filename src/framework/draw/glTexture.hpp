#ifndef _GLTEXTURE_HPP
#define _GLTEXTURE_HPP

#include <GL/gl.h>
#include "image.hpp"

class GLTexture
{
    GLuint id;
    unsigned w, h;

    public:
    enum Format
    {
        RGB8,
        RGBA8,
        RGBA8I,
        RGB32F,
        RGBA32F,
        RGB10A2,
        DEPTH32,
        DEPTH24_STENCIL8,
    };

    unsigned getWidth() { return w; }
    unsigned getHeight() { return h; }

    GLTexture(unsigned w, unsigned h, int format);
    //GLTexture(Image &i);
    GLTexture(Image i);

    Image asImage();

    friend class GLDrawProgram;
    friend class GLDrawDevice;
    friend class GLFramebuffer;
};

#endif
