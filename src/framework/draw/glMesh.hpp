#ifndef _GLMESH_HPP
#define _GLMESH_HPP

#include "mesh.hpp"

#define GL_GLEXT_PROTOTYPES

#include <GL/gl.h>
#include <GL/glext.h>

class GLMesh
{
    public:
    GLuint vbuffer;
    GLuint ibuffer;
    GLuint vao;

    unsigned nelem;

    public:

    GLMesh(Mesh &m);
    unsigned getNElements() { return nelem; }

    virtual ~GLMesh();

    friend class GLDrawDevice;
    friend class GLDrawProgram;
};

#endif
