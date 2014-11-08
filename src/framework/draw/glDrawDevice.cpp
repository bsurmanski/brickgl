#include "glDrawDevice.hpp"

#include <stdint.h>
#include <math.h>
#include <stdio.h>

// builtin fullscreen quad, and deferred shaders

static float quadv[] = {
    -1, -1, 1,  0, 0, -1,  0, 0,
    1, -1, 1,   0, 0, -1,  1, 0,
    1, 1, 1,    0, 0, -1,  1, 1,

    -1, -1, 1,  0, 0, -1,  0, 0,
    1, 1, 1,    0, 0, -1,  1, 1,
    -1, 1, 1,   0, 0, -1,  0, 1,
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
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glDisable(GL_BLEND);
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_SCISSOR_TEST);
    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
    glEnable(GL_DEPTH_CLAMP);

    // full screen quad
    glGenBuffers(1, &vquad);
    glBindBuffer(GL_ARRAY_BUFFER, vquad);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float[8]) * 6, quadv, GL_STATIC_DRAW);

    deferredProgram = new GLDrawProgram;
    GLDrawShader *dvs = GLDrawShader::fromString(GLDrawShader::VERTEX_SHADER, deferredvs);
    GLDrawShader *dfs = GLDrawShader::fromString(GLDrawShader::FRAGMENT_SHADER, deferredfs);
    deferredProgram->bindStage(0, dvs);
    deferredProgram->bindStage(1, dfs);

    mainProgram = (GLDrawProgram*) this->createProgram();
    const char mainvs[] = {
        #include "../../glsl/mesh.vs.h"
    };
    const char mainfs[] = {
        #include "../../glsl/mesh.fs.h"
    };

    width = 640;
    height = 480;
    mainProgram = GLDrawProgram::fromVFShaderStrings(mainvs, mainfs);

    GLTexture *lightTexture = new GLTexture(width, height, GLTexture::RGBA8);
    mainBuffer = new GLFramebuffer;
    mainBuffer->appendTarget(new GLTexture(width, height, GLTexture::RGBA8)); //color
    mainBuffer->appendTarget(new GLTexture(width, height, GLTexture::RGBA8I)); //normal
    mainBuffer->appendTarget(new GLTexture(width, height, GLTexture::RGBA32F)); //position
    mainBuffer->appendTarget(lightTexture);
    mainBuffer->setDepth(new GLTexture(width, height, GLTexture::DEPTH32)); // depth

    const char lightvs[] = {
        #include "../../glsl/light.vs.h"
    };

    const char lightfs[] = {
        #include "../../glsl/light.fs.h"
    };

    lightProgram = GLDrawProgram::fromVFShaderStrings(lightvs, lightfs);
    lightProgram->setAccum(true);
    lightBuffer = new GLFramebuffer;
    lightBuffer->appendTarget(lightTexture);


    mainProgram->use();
}

GLDrawDevice::~GLDrawDevice()
{
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
    glVertexAttribPointer(pos_uint, 3, GL_FLOAT, GL_FALSE, 32, 0);

    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
}

void GLDrawDevice::drawToScreen()
{
    GLTexture *color = mainBuffer->getTarget(0);
    GLTexture *normal = mainBuffer->getTarget(1);
    GLTexture *depth = mainBuffer->getDepth();
    GLTexture *light = lightBuffer->getTarget(0);
    deferredProgram->use();
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable(GL_BLEND);

    bindTexture(0, color);
    bindTexture(1, normal);
    bindTexture(2, depth);
    bindTexture(3, light);
    deferredProgram->setUniform("t_color", 0);
    deferredProgram->setUniform("t_normal", 1);
    deferredProgram->setUniform("t_depth", 2);
    deferredProgram->setUniform("t_light", 3);

    drawFullscreenQuad();

    // clean up state
    glActiveTexture(GL_TEXTURE0);
}

/*
void drawToTexture(GLTexture *dest,
                    GLTexture *color,
                    GLTexture *normal,
                    GLTexture *depth,
                    GLTexture *light)
{

}*/

void GLDrawDevice::applyLighting()
{
    static float angle = 0.5f;
    //angle += 0.05f;
    mat4 mMatrix = mat4::getTranslation(vec4(5,0,0,0));
    mat4 vMatrix = camera.getView();
    mat4 mvpMatrix = camera.getPerspective() * vMatrix * mMatrix;

    vec4 pos1 = vec4(10.0f, 1000.0f, -500.0f, 1.0f);
    vec4 pos2 = vec4(sin(angle) * -10.0f, 10.0f, -10.0f, 1.0f);
    vec4 pos3 = vec4(sin(angle) * 10.0f + 30.0f, sin(angle) * 10.0f + 10.0f, -10.0f, 1.0f);

    lightProgram->use();
    lightBuffer->bind();
    //glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    //glClear(GL_COLOR_BUFFER_BIT);

    vec4 lightPos = vec4(10.0f, 1000.0f, -500.0f, 1.0f);
    drawLight(vec4(10.0f, 1000.0f, -500.0f, 1.0f), vec4(1,1,1,1), 0.5);
    drawLight(vec4(sin(angle) * -10.0f, 10.0f, -10.0f, 1.0f), vec4(1,1,1,1), 0.3);
    drawLight(vec4(sin(angle) * 10.0f + 30.0f, sin(angle) * 10.0f + 10.0f, -10.0f, 1.0f),
            vec4(1,1,1,1), 0.3);
}

void GLDrawDevice::drawLight(vec4 loc, vec4 color, float brightness)
{
    mat4 mMatrix = mat4::getTranslation(vec4(5,0,0,0));
    mat4 vMatrix = camera.getView();
    mat4 mvpMatrix = camera.getPerspective() * vMatrix * mMatrix;

    lightProgram->use();
    lightBuffer->bind();

    vec4 camPos = mat4::getRotation(camera.getRotation()) * (camera.getOffset());
    camPos.z = -camPos.z;

    lightProgram->setUniform("light", loc);
    lightProgram->setUniform("camera", camPos);
    lightProgram->setUniform("color", color * brightness);
    lightProgram->bindTexture("t_normal", 0, mainBuffer->getTarget(1));
    lightProgram->bindTexture("t_position", 1, mainBuffer->getTarget(2));
    lightProgram->bindTexture("t_depth", 2, mainBuffer->getDepth());
    this->drawFullscreenQuad();
}

void GLDrawDevice::drawMesh(GLMesh *mesh, GLTexture *tex, mat4 mMatrix)
{
    mat4 vMatrix = camera.getView();

    mat4 mvpMatrix = camera.getPerspective() * vMatrix * mMatrix;

    mainProgram->use();
    mainBuffer->bind();

    mainProgram->setUniform("mvpMatrix", mvpMatrix);
    mainProgram->setUniform("mMatrix", mMatrix);
    mainProgram->setUniform("ambient", vec4(0.1, 0.1, 0.1, 0));

    mainProgram->bindTexture("t_color", 0, tex);

    mainProgram->drawMesh(mesh);
}

void GLDrawDevice::drawMeshUnlit(GLMesh *mesh, GLTexture *tex, mat4 mMatrix)
{
    mat4 vMatrix = camera.getView();

    mat4 mvpMatrix = camera.getPerspective() * vMatrix * mMatrix;

    mainProgram->use();
    mainBuffer->bind();

    mainProgram->setUniform("mvpMatrix", mvpMatrix);
    mainProgram->setUniform("mMatrix", mMatrix);
    mainProgram->setUniform("ambient", vec4(1,1,1,0));

    mainProgram->bindTexture("t_color", 0, tex);
    mainProgram->drawMesh(mesh);
}

void GLDrawDevice::drawFlat(GLTexture *tex, vec4 loc, vec4 scale)
{
    mat4 mMatrix = mat4::getTranslation(vec4(-1, -1, 0, 0)) *
        mat4::getScale(
            vec4((float) tex->getWidth() / width, (float) tex->getHeight() / height, 1, 1)) *
        mat4::getTranslation(vec4(1, 1, 0, 0));
    mat4 mvpMatrix = mMatrix;

    mainProgram->use();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    mainProgram->setUniform("mvpMatrix", mvpMatrix);
    mainProgram->setUniform("mMatrix", mMatrix);
    mainProgram->bindTexture("t_color", 0, tex);

    glBindBuffer(GL_ARRAY_BUFFER, vquad);
    GLuint pos_uint = glGetAttribLocation(mainProgram->id, "position");
    GLuint uv_uint = glGetAttribLocation(mainProgram->id, "uv");
    glEnableVertexAttribArray(pos_uint);
    glEnableVertexAttribArray(uv_uint);
    glVertexAttribPointer(pos_uint, 3, GL_FLOAT, GL_FALSE, 32, 0);
    glVertexAttribPointer(uv_uint, 2, GL_FLOAT, GL_FALSE, 32, (void*) 24);

    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
}

Image GLDrawDevice::screenshot() {
    Image i;
    i.w = width;
    i.h = height;
    i.pixels = (uint8_t*) malloc(width * height * 4); //TODO: free
    glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, i.pixels);

    // do some stupid dance to swap R and B components of color
    // because OpenGL outputs ABGR or something silly
    uint32_t *pxl = (uint32_t*) i.pixels;
    for(int i = 0; i < width * height; i++) {
        int R = pxl[i] & 0x00ff0000;
        int B = pxl[i] & 0x000000ff;
        pxl[i] = (pxl[i] & 0xff00ff00) | (R >> 16) | (B << 16);
    }

    return i;
}
