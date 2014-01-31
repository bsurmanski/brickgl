#ifndef _GLDRAWPROGRAM_HPP
#define _GLDRAWPROGRAM_HPP

#include "drawProgram.hpp"
#include "glDrawShader.hpp"

class GLDrawProgram : public DrawProgram
{
    GLuint id; 
    GLDrawShader *shaders[5];

    void clean();

    public:
    GLDrawProgram();
    virtual ~GLDrawProgram();
    void bindStage(int stage, GLDrawShader *program);
};

#endif
