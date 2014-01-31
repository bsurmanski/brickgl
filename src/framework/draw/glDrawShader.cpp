#include "glDrawShader.hpp"

#include <iostream>
#include <assert.h>

GLDrawShader *GLDrawShader::fromString(unsigned stage, const char *str, int len)
{
    GLDrawShader *dshader = new GLDrawShader;

    GLuint sh = glCreateShader(stage);
    glShaderSource(sh, 1, &str, &len);
    GLint cstatus;
    glGetShaderiv(sh, GL_COMPILE_STATUS, &cstatus);
    if(cstatus == GL_FALSE)
    {
        char buf[512];
        glGetShaderInfoLog(sh, 512, 0, buf);
        std::cout << buf; 
        assert(false); //TODO: handle error
    }

    dshader->id = sh;
    return dshader;
}

GLDrawShader *GLDrawShader::fromFile(unsigned stage, const char *filenm)
{
    return NULL;
}
