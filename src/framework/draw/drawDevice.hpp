#ifndef _DRAWDEVICE_HPP
#define _DRAWDEVICE_HPP

#include "../../matrix.hpp"
#include "../device.hpp"
#include "drawProgram.hpp"

//TODO: should no reference GL stuff
#include "glMesh.hpp"
#include "glTexture.hpp"

class DrawDevice : public Device
{
    public:
    DrawDevice();
    virtual ~DrawDevice();
    virtual void update(float dt);
    virtual DrawProgram *createProgram() = 0;

    virtual void drawFullscreenQuad() = 0;
    virtual void drawToScreen() = 0;
    virtual void applyLighting() = 0;
    virtual void drawMesh(GLMesh *mesh, GLTexture *tex, mat4 mMatrix) = 0;
};

#endif
