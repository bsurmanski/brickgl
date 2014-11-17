#include "glCubemap.hpp"

void GLCubemap::initialize(Image *img[6]) {
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_CUBE_MAP, id);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_REPEAT);

    if(!img[0]) throw;

    w = img[0]->w;
    h = img[0]->h;
    unsigned iformat = GLDraw::getGLInternalFormat(img[0]->format);
    unsigned format = GLDraw::getGLFormat(img[0]->format);
    unsigned gltype = GLDraw::getGLType(img[0]->format);

    unsigned targets[6] = {
        GL_TEXTURE_CUBE_MAP_POSITIVE_X,
        GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
        GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
        GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
        GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
        GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
    };

    for(int i = 0; i < 6; i++) {
        if(!img[i] || img[i]->w != w || img[i]->h != h ||
            GLDraw::getGLInternalFormat(img[i]->format) != iformat) {
            throw;
        }

        glTexImage2D(targets[i], 0, iformat, w, h, 0, format,
                gltype, img[i]->pixels);
    }
}

GLCubemap::GLCubemap(Image *img[6]) {
    initialize(img);
}


GLCubemap::GLCubemap(Image &xp, Image &xn, Image &yp, Image &yn, Image &zp, Image &zn) {
    Image* img[6] = {&xp, &xn, &yp, &yn, &zp, &zn};
    initialize(img);
}
