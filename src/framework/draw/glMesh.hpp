#ifndef _GLMESH_HPP
#define _GLMESH_HPP

#include "mesh.hpp"

#define GL_GLEXT_PROTOTYPES

#include <GL/gl.h>
#include <GL/glext.h>

struct GLVertex;
struct GLIndex;

class GLMesh
{
    static GLMesh *cubeMesh;
    static GLMesh *quadMesh;

    GLMesh(GLVertex *v, int nv, GLIndex *i, int ni);

    public:
    GLuint vbuffer;
    GLuint ibuffer;
    GLuint vao;

    unsigned nelem;

    public:

    GLMesh(Mesh &m);
    static GLMesh *getUnitCube();
    static GLMesh *getUnitQuad();
    unsigned getNElements() { return nelem; }

    virtual ~GLMesh();

    friend class GLDrawDevice;
    friend class GLDrawProgram;
};

#endif
