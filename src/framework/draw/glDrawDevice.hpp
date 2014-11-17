#ifndef _GLDRAWDEVICE_HPP
#define _GLDRAWDEVICE_HPP

#define GL_GLEXT_PROTOTYPES

#include <GL/gl.h>
#include <GL/glext.h>

#include "drawDevice.hpp"
#include "glFramebuffer.hpp"
#include "glDrawProgram.hpp"
#include "glTexture.hpp"
#include "glCubemap.hpp"
#include "camera.hpp"

class GLDrawDevice : public DrawDevice
{
    public:
    GLDrawProgram *deferredProgram;
    GLDrawProgram *mainProgram;
    GLDrawProgram *lightProgram;
    GLDrawProgram *skyboxProgram;
    GLFramebuffer *mainBuffer;
    GLFramebuffer *lightBuffer;

    Camera camera;

    unsigned width;
    unsigned height;

    public:
    GLDrawDevice();
    virtual ~GLDrawDevice();
    virtual DrawProgram *createProgram();
    virtual void bindTexture(unsigned unit, GLTexture *tex);
    virtual void drawFullscreenQuad();
    virtual void drawToScreen();

    virtual void applyLighting();
    virtual void drawLight(vec4 loc, vec4 color, float brightness=1.0f);
    virtual void drawSkybox(GLCubemap *tex);
    virtual void drawMesh(GLMesh *mesh, GLTexture *tex, mat4 mMatrix);
    virtual void drawMeshUnlit(GLMesh *mesh, GLTexture *tex, mat4 mMatrix);
    virtual void drawFlat(GLTexture *tex, vec4 loc, vec4 scale = vec4(1,1,1,1));

    Image screenshot();
};

#endif
