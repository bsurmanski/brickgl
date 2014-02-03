#include "glDrawProgram.hpp"

#include <string.h>

GLDrawProgram::GLDrawProgram()
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

void GLDrawProgram::bindStage(int stage, GLDrawShader *program)
{
    if(shaders[stage])
    {
        glDetachShader(id, shaders[stage]->id);
    }

    shaders[stage] = program;
    //clean();
}

void GLDrawProgram::use()
{
    glUseProgram(id);
}
