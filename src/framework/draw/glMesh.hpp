#ifndef _GLMESH_HPP
#define _GLMESH_HPP

#include "mesh.hpp"

#define GL_GLEXT_PROTOTYPES

#include <GL/gl.h>
#include <GL/glext.h>

class GLMesh : public Mesh
{
    GLuint ibuffer;
    GLuint vbuffer;
    GLuint vao;

    virtual ~GLMesh(){}
};

#endif
