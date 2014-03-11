#ifndef _GLDRAWDEVICE_HPP
#define _GLDRAWDEVICE_HPP

#define GL_GLEXT_PROTOTYPES

#include <GL/gl.h>
#include <GL/glext.h>

#include "drawDevice.hpp"
#include "glFramebuffer.hpp"
#include "glDrawProgram.hpp"
#include "glTexture.hpp"

class GLDrawDevice : public DrawDevice
{
    GLDrawProgram *deferredProgram;
    GLDrawProgram *mainProgram;
    GLDrawProgram *lightProgram;

    public:
    GLDrawDevice();
    virtual ~GLDrawDevice();
    virtual DrawProgram *createProgram();
    virtual void bindTexture(unsigned unit, GLTexture *tex);
    virtual void drawFullscreenQuad();
    virtual void drawToScreen(GLTexture *color, GLTexture *normal, GLTexture *depth, GLTexture *light);
    /*
    virtual void drawToTexture(GLTexture *dest, GLTexture *color,
                    GLTexture *normal, GLTexture *depth, GLTexture *light);
*/
};

#endif
