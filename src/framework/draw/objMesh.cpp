#include "objMesh.hpp"
#include "mesh.hpp"
#include <stdio.h>

using namespace std;

int fpeek(FILE *file)
{
    char c = fgetc(file);
    ungetc(c, file);
    return c;
}

void fdropline(FILE *file)
{
    while(fgetc(file) != '\n');
}

Mesh *loadObjMesh(std::string filenm)
{
    FILE *file = fopen(filenm.c_str(), "r");
    vector<MeshVertex> verts;
    vector<MeshFace> faces;

    while(!feof(file))
    {
        char c = fpeek(file);
        if(c == '#') // skip comment
        {
            fdropline(file);
            continue;
        }
        else if(c == '\n' || c == '\r')
        {
            fgetc(file);
            continue;
        }
        else if(c == 'o')
        {
            fdropline(file);
            continue;
        }
        else if(c == 's')
        {
            fdropline(file);
            continue;
        } else if(c == 'v')
        {
            float v[3];
            fscanf(file, "v %f %f %f", &v[0], &v[1], &v[2]);
            verts.push_back(MeshVertex(v[0], v[1], v[2]));
        } else if(c == 'f')
        {
            int f[4];
            fscanf(file, "f %i %i %i %i", &f[0], &f[1], &f[2], &f[3]);
            faces.push_back(MeshFace(f[0], f[1], f[2]));
            faces.push_back(MeshFace(f[0], f[2], f[3]));
        } else if(c == 'm')
        {
            //mtllib
            fdropline(file);
        } else if(c == 'u')
        {
            //usemtl
            fdropline(file);
        }
    }

    return new Mesh(verts, faces);
}
