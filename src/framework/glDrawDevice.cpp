#include "glDrawDevice.hpp"

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
