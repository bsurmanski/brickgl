#ifndef _MESH_HPP
#define _MESH_HPP

#include<vector>

struct MeshPosition
{
    float v[3];
    MeshPosition(float x, float y, float z);
};

struct MeshUv
{
    float v[2];
    MeshUv(float x, float y) { v[0] = x; v[1] = y; }
};

struct MeshNormal
{
    float v[3];
    MeshNormal(float x, float y, float z) { v[0] = x; v[1] = y; v[2] = z; }
};

struct MeshFace
{
    unsigned short v[3]; //vert
    unsigned short n[3]; //normal
    unsigned short uv[3]; //uv
    MeshFace(unsigned short a, unsigned short b, unsigned short c) {
        v[0] = a;
        v[1] = b;
        v[2] = c;
        n[0] = n[1] = n[2] = 0;
        uv[0] = uv[1] = uv[2] = 0;
    }

    MeshFace(unsigned short v0, unsigned short v1, unsigned short v2,
            unsigned short n0, unsigned short n1, unsigned short n2,
            unsigned short uv0, unsigned short uv1, unsigned short uv2) {
        v[0] = v0;
        v[1] = v1;
        v[2] = v2;
        n[0] = n0;
        n[1] = n1;
        n[2] = n2;
        uv[0] = uv0;
        uv[1] = uv1;
        uv[2] = uv2;
    }

};

class Mesh
{
    public:

    std::vector<MeshPosition> verts;
    std::vector<MeshUv> uvs;
    std::vector<MeshNormal> norms;
    std::vector<MeshFace> faces;

    static int vertexStride() { return sizeof(MeshPosition); }

    Mesh();
    unsigned nverts() { return verts.size(); }
    unsigned nfaces() { return faces.size(); }
    std::vector<MeshPosition>* getVerts() { return &verts; }
    std::vector<MeshFace>* getFaces() { return &faces; }

    void addVert(MeshPosition p) { verts.push_back(p); }
    void addUv(MeshUv u) { uvs.push_back(u); }
    void addNorm(MeshNormal n) { norms.push_back(n); }
    void addFace(MeshFace f) { faces.push_back(f); }

    virtual ~Mesh();
};

#endif
