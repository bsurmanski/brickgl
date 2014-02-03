#include "mesh.hpp"

MeshVertex::MeshVertex(float x, float y, float z)
{
    v[0] = x;
    v[1] = y;
    v[2] = z;
}

MeshFace::MeshFace(unsigned short a, unsigned short b, unsigned short c)
{
    v[0] = a;
    v[1] = b;
    v[2] = c;
}

Mesh::Mesh()
{

}

Mesh::Mesh(std::vector<MeshVertex> v, std::vector<MeshFace> f)
{
    verts = v;
    faces = f;
}
