#ifndef _GLDRAWPROGRAM_HPP
#define _GLDRAWPROGRAM_HPP

#include "drawProgram.hpp"
#include "glDrawShader.hpp"
#include "glTexture.hpp"

#include "glFramebuffer.hpp"

#include "../../matrix.hpp"
#include "../../vector.hpp"

#include "glMesh.hpp"

#include <vector>

class GLDrawProgram : public DrawProgram
{
    bool dirty;
    void clean();

    GLDrawShader *shaders[5];
    GLFramebuffer *destination;

    //std::vector<std::pair<GLuint, GLTexture*> > textures;


    public:
    GLuint id; 
    static GLDrawProgram *bound;
    GLDrawProgram();
    virtual ~GLDrawProgram();
    void bindStage(int stage, GLDrawShader *program);
    void bindTexture(const char *name, unsigned unit, GLTexture *texture);
    void setDestination(GLFramebuffer *fb); //TODO
    void use();

    void drawMesh(GLMesh *mesh);

    void setUniform(const char *nm, int val);
    void setUniform(const char *nm, mat4 &matrix);
};

#endif
