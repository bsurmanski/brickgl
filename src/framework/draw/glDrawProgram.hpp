#ifndef _GLDRAWPROGRAM_HPP
#define _GLDRAWPROGRAM_HPP

#include "drawProgram.hpp"
#include "glDrawShader.hpp"

class GLDrawProgram : public DrawProgram
{
    public: //TODO: shouldnt be public?
    GLuint id; 
    GLDrawShader *shaders[5];

    void clean();

    public:
    GLDrawProgram();
    virtual ~GLDrawProgram();
    void bindStage(int stage, GLDrawShader *program);
};

#endif
