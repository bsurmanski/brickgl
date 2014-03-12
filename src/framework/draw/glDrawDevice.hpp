#ifndef _GLDRAWDEVICE_HPP
#define _GLDRAWDEVICE_HPP

#define GL_GLEXT_PROTOTYPES

#include <GL/gl.h>
#include <GL/glext.h>

#include "drawDevice.hpp"
#include "glFramebuffer.hpp"
#include "glDrawProgram.hpp"
#include "glTexture.hpp"
#include "camera.hpp"

class GLDrawDevice : public DrawDevice
{
    public:
    GLDrawProgram *deferredProgram;
    GLDrawProgram *mainProgram;
    GLDrawProgram *lightProgram;
    GLFramebuffer *mainBuffer;
    GLFramebuffer *lightBuffer;

    Camera camera;

    public:
    GLDrawDevice();
    virtual ~GLDrawDevice();
    virtual DrawProgram *createProgram();
    virtual void bindTexture(unsigned unit, GLTexture *tex);
    virtual void drawFullscreenQuad();
    virtual void drawToScreen();

    virtual void applyLighting();
    virtual void drawMesh(GLMesh *mesh, GLTexture *tex, mat4 mMatrix);
};

#endif
