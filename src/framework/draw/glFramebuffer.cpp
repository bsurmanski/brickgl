#include "glFramebuffer.hpp"

#include <GL/glext.h>

#include <stdio.h>

GLFramebuffer *GLFramebuffer::bound = 0;

static unsigned glBuffers[] = 
{
    GL_COLOR_ATTACHMENT0,
    GL_COLOR_ATTACHMENT1,
    GL_COLOR_ATTACHMENT2,
    GL_COLOR_ATTACHMENT3,
    GL_COLOR_ATTACHMENT0,
    GL_COLOR_ATTACHMENT0,
    GL_COLOR_ATTACHMENT4,
    GL_COLOR_ATTACHMENT5,
    GL_COLOR_ATTACHMENT6,
    GL_COLOR_ATTACHMENT7,
    GL_COLOR_ATTACHMENT8,
    GL_COLOR_ATTACHMENT9,
};

GLFramebuffer::GLFramebuffer()
{
    glGenFramebuffers(1, &id);
}

void GLFramebuffer::bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, id);
    glDrawBuffers(targets.size(), glBuffers);
}

GLTexture *GLFramebuffer::getDepth()
{
    return depth;
}

void GLFramebuffer::setDepth(GLTexture *t)
{
    GLFramebuffer *prev = bound;

    glBindFramebuffer(GL_FRAMEBUFFER, id);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, // TODO: depth stencil
            GL_TEXTURE_2D, t->id, 0);

    depth = t;

    if(prev)
        glBindFramebuffer(GL_FRAMEBUFFER, prev->id);
}

void GLFramebuffer::appendTarget(GLTexture *t)
{
    GLFramebuffer *prev = bound;

    glBindFramebuffer(GL_FRAMEBUFFER, id);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + targets.size(), 
            GL_TEXTURE_2D, t->id, 0);

    targets.push_back(t);

    if(prev)
        glBindFramebuffer(GL_FRAMEBUFFER, prev->id);
}

GLTexture *GLFramebuffer::getTarget(unsigned i)
{
    return targets[i];
}
