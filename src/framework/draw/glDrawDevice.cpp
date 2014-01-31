#include "glDrawDevice.hpp"

#include <stdint.h>

static float fsquad_verts[] = {
    -1, -1, 0,
    1, -1, 0,
    1, 1, 0,
    -1, 1, 0
};
static unsigned short fsquad_faces[] = {0, 1, 2, 0, 2, 3};

GLDrawDevice::GLDrawDevice()
{
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glDisable(GL_BLEND);
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_SCISSOR_TEST);
    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
    glDepthFunc(GL_LEQUAL);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

GLDrawDevice::~GLDrawDevice()
{
    this->~DrawDevice();
}

DrawProgram *GLDrawDevice::createProgram()
{
    return new GLDrawProgram;
}
