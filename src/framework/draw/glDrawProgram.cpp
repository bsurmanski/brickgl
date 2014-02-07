#include "glDrawProgram.hpp"

#include <string.h>

GLDrawProgram *GLDrawProgram::bound = 0;

GLDrawProgram::GLDrawProgram() : destination(0)
{
    id = glCreateProgram();
    memset(shaders, 0, sizeof(GLDrawShader*[5]));
}

GLDrawProgram::~GLDrawProgram()
{

}

void GLDrawProgram::clean()
{
    for(int i = 0; i < 5; i++)
    {
        if(shaders[i])
        {
            glAttachShader(id, shaders[i]->id);
        }
    }

    glLinkProgram(id);
}

static const unsigned glBuffers[] =
{
    GL_COLOR_ATTACHMENT0,
    GL_COLOR_ATTACHMENT1,
    GL_COLOR_ATTACHMENT2,
    GL_COLOR_ATTACHMENT3,
    GL_COLOR_ATTACHMENT4,
    GL_COLOR_ATTACHMENT5,
    GL_COLOR_ATTACHMENT6,
    GL_COLOR_ATTACHMENT7,
    GL_COLOR_ATTACHMENT8,
    GL_COLOR_ATTACHMENT9,
};

void GLDrawProgram::setDestination(GLFramebuffer *fb)
{
    destination = fb; 
}

void GLDrawProgram::bindTexture(char *name, GLTexture *texture)
{
    GLuint texloc = glGetUniformLocation(id, name);
    textures.push_back(std::pair<GLuint, GLTexture*>(texloc, texture)); 

    glUseProgram(id);
    glUniform1i(texloc, texture->id);
}

void GLDrawProgram::bindStage(int stage, GLDrawShader *program)
{
    if(shaders[stage])
    {
        glDetachShader(id, shaders[stage]->id);
    }

    shaders[stage] = program;
}

void GLDrawProgram::use()
{
    glUseProgram(id);
    for(int i = 0; i < textures.size(); i++)
    {
        glUniform1i(textures[i].first, textures[i].second->id);
    }

    if(destination) destination->bind();
    else 
    {
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
        glDrawBuffers(1, glBuffers);
    }

    bound = this;
}
