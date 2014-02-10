#include "mesh.hpp"

MeshPosition::MeshPosition(float x, float y, float z)
{
    v[0] = x;
    v[1] = y;
    v[2] = z;
}

Mesh::Mesh()
{

}

Mesh::~Mesh()
{
    //vectors should dealloc automatically
}
