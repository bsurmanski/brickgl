#ifndef _GLDRAWSHADER_HPP
#define _GLDRAWSHADER_HPP

#include <stddef.h>
#include <stdint.h>
#include <string>

#define GL_GLEXT_PROTOTYPES

#include <GL/gl.h>
#include <GL/glext.h>

class GLDrawShader
{
    GLuint id;

    public:
    enum
    {
        GEOMETRY_SHADER = GL_GEOMETRY_SHADER, 
        VERTEX_SHADER = GL_VERTEX_SHADER,
        FRAGMENT_SHADER = GL_FRAGMENT_SHADER,
    };

        GLDrawShader(unsigned stage);

        static GLDrawShader *fromString(unsigned stage, const char *str, int len = 0);
        static GLDrawShader *fromFile(unsigned stage, const char *filenm);
        friend class GLDrawProgram;
};

#endif
