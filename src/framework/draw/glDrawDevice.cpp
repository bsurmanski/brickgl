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
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glDisable(GL_BLEND);
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_SCISSOR_TEST);
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
