#include "glDrawShader.hpp"

#include <iostream>
#include <assert.h>

GLDrawShader::GLDrawShader(unsigned stage)
{
    id = glCreateShader(stage);
}

GLDrawShader *GLDrawShader::fromString(unsigned stage, const char *str, int len)
{
    GLDrawShader *shader = new GLDrawShader(stage);

    glShaderSource(shader->id, 1, &str, NULL);
    glCompileShader(shader->id);

    GLint cstatus;
    glGetShaderiv(shader->id, GL_COMPILE_STATUS, &cstatus);
    if(cstatus == GL_FALSE)
    {
        char buf[512];
        glGetShaderInfoLog(shader->id, 512, 0, buf);
        std::cout << buf << std::endl;
        assert(false); //TODO: handle error
    }

    return shader;
}

GLDrawShader *GLDrawShader::fromFile(unsigned stage, const char *filenm)
{
    return NULL;
}
