#ifndef _GLDRAWPROGRAM_HPP
#define _GLDRAWPROGRAM_HPP

#include "drawProgram.hpp"
#include "glDrawShader.hpp"
#include "glTexture.hpp"

#include "glFramebuffer.hpp"

#include <vector>

class GLDrawProgram : public DrawProgram
{
    public: //TODO: shouldnt be public?
    static GLDrawProgram *bound;

    GLuint id; 
    GLDrawShader *shaders[5];
    GLFramebuffer *destination;

    std::vector<std::pair<GLuint, GLTexture*> > textures;

    void clean();

    public:
    GLDrawProgram();
    virtual ~GLDrawProgram();
    void bindStage(int stage, GLDrawShader *program);
    void bindTexture(char *name, GLTexture *texture);
    void setDestination(GLFramebuffer *fb); //TODO
    void use();
};

#endif
