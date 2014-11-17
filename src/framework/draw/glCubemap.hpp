#ifndef _GLCUBEMAP_HPP
#define _GLCUBEMAP_HPP

#include <GL/gl.h>

#include "glTexture.hpp"

class GLCubemap {
    GLuint id;
    unsigned w, h;

    void initialize(Image *img[6]);

    public:
    unsigned getWidth() { return w; }
    unsigned getHeight() { return h; }
    GLuint getId() { return id; }

    GLCubemap(Image &xp, Image &xn, Image &yp, Image &yn, Image &zp, Image &zn);
    GLCubemap(Image *img[6]);
};

#endif
