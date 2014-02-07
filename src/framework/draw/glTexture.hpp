#ifndef _GLTEXTURE_HPP
#define _GLTEXTURE_HPP

#include <GL/gl.h>
#include "image.hpp"

class GLTexture
{
    GLuint id;
    public:
    enum Format
    {
        DEPTH24_STENCIL8,
        DEPTH32,
        RGBA8,
        RGB8
    };

    GLTexture(unsigned w, unsigned h, int format);
    GLTexture(Image &i);
    
    friend class GLDrawProgram;
    friend class GLDrawDevice;
    friend class GLFramebuffer;
};

#endif
