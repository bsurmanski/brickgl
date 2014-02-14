#include "glDrawDevice.hpp"

#include <stdint.h>

// builtin fullscreen quad, and deferred shaders

static float quadv[] = {
    -1, -1, 0,
    1, -1, 0,
    1, 1, 0,

    -1, -1, 0,
    1, 1, 0,
    -1, 1, 0
};
static GLuint vquad;

static const char deferredvs[] =
{
    #include "../../glsl/deferred.vs.h"
};

static const char deferredfs[] =
{
    #include "../../glsl/deferred.fs.h"
};

GLDrawDevice::GLDrawDevice()
{
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glDisable(GL_BLEND);
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_SCISSOR_TEST);
    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

    // full screen quad
    glGenBuffers(1, &vquad);
    glBindBuffer(GL_ARRAY_BUFFER, vquad);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float[18]), quadv, GL_STATIC_DRAW);

    deferred = new GLDrawProgram;
    GLDrawShader *dvs = GLDrawShader::fromString(GLDrawShader::VERTEX_SHADER, deferredvs);
    GLDrawShader *dfs = GLDrawShader::fromString(GLDrawShader::FRAGMENT_SHADER, deferredfs);
    deferred->bindStage(0, dvs);
    deferred->bindStage(1, dfs);
}

GLDrawDevice::~GLDrawDevice()
{
    this->~DrawDevice();
}

void GLDrawDevice::bindTexture(unsigned unit, GLTexture *tex)
{
    unsigned tid = 0;
    if(tex) tid = tex->id;
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, tid);
}

DrawProgram *GLDrawDevice::createProgram()
{
    return new GLDrawProgram;
}

void GLDrawDevice::drawFullscreenQuad()
{
    if(!GLDrawProgram::bound) return;
    glBindBuffer(GL_ARRAY_BUFFER, vquad);
    GLuint pos_uint = glGetAttribLocation(GLDrawProgram::bound->id, "position");
    glEnableVertexAttribArray(pos_uint);
    glVertexAttribPointer(pos_uint, 3, GL_FLOAT, GL_FALSE, 12, 0);

    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
}

void GLDrawDevice::drawToScreen(GLTexture *color, GLTexture *normal, GLTexture *depth, GLTexture *light)
{
    deferred->use();
    glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable(GL_BLEND);

    bindTexture(0, color);
    bindTexture(1, normal);
    bindTexture(2, depth);
    bindTexture(3, light);
    deferred->setUniform("t_color", 0);
    deferred->setUniform("t_normal", 1);
    deferred->setUniform("t_depth", 2);
    deferred->setUniform("t_light", 3);

    drawFullscreenQuad();

    // clean up state
    glActiveTexture(GL_TEXTURE0);
}
