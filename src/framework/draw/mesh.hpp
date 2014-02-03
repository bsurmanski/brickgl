#ifndef _MESH_HPP
#define _MESH_HPP

#include<vector>

struct MeshVertex
{
    float v[3];
    MeshVertex(float x, float y, float z);
};

struct MeshFace
{
    unsigned short v[3];
    MeshFace(unsigned short a, unsigned short b, unsigned short c);
};

class Mesh
{
    public:

    std::vector<MeshVertex> verts;
    std::vector<MeshFace> faces;

    Mesh();
    Mesh(std::vector<MeshVertex> v, std::vector<MeshFace> f);

    virtual ~Mesh(){}
};

#endif
