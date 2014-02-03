#include "glMesh.hpp"

GLMesh::GLMesh(Mesh &m)
{
    glGenBuffers(1, &ibuffer);
    glGenBuffers(1, &vbuffer);

    glBindBuffer(GL_ARRAY_BUFFER, vbuffer);
    glBufferData(GL_ARRAY_BUFFER, 
            sizeof(MeshVertex) * m.verts.size(), 
            m.verts.data(),
            GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 
            sizeof(MeshFace) * m.faces.size(), 
            m.faces.data(),
            GL_STATIC_DRAW);

    nelem = m.faces.size() * 3;
}
