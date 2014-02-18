#ifndef _GLFRAMEBUFFER_HPP
#define _GLFRAMEBUFFER_HPP

#define GL_GLEXT_PROTOTYPES

#include <vector>
#include <GL/gl.h>

#include "glTexture.hpp"

class GLFramebuffer
{
    GLuint id;
    GLTexture *depth;
    std::vector<GLTexture*> targets;

    static GLFramebuffer *bound;

    public:
    GLFramebuffer();
    void bind();
    void clear();

    GLTexture *getDepth();
    void setDepth(GLTexture *t);

    void appendTarget(GLTexture *t);
    GLTexture *getTarget(unsigned i);

    friend class GLDrawDevice;
};

#endif
