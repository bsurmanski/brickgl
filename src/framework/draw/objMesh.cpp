#include "objMesh.hpp"
#include "mesh.hpp"

#include <stdint.h>
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

Mesh loadObjMesh(std::string filenm)
{
    FILE *file = fopen(filenm.c_str(), "r");
    Mesh m;

    if(!file) return m;

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
            fgetc(file); // ignore v
            if(fpeek(file) == 't') // uv
            {
                fscanf(file, "t %f %f", &v[0], &v[1]);
                m.uvs.push_back(MeshUv(v[0], v[1]));
                continue;
            } else if(fpeek(file) == 'n')
            {

                fscanf(file, "n %f %f %f", &v[0], &v[1], &v[2]);
                m.norms.push_back(MeshNormal(v[0], v[1], v[2]));
                continue;
            }

            fscanf(file, " %f %f %f", &v[0], &v[1], &v[2]);
            m.verts.push_back(MeshPosition(v[0], v[1], v[2]));
        } else if(c == 'f')
        {
            uint16_t vi[4];
            uint16_t ni[4];
            uint16_t ui[4];

            fscanf(file, "f %hu/%hu/%hu %hu/%hu/%hu %hu/%hu/%hu %hu/%hu/%hu", 
                    &vi[0], &ui[0], &ni[0],
                    &vi[1], &ui[1], &ni[1],
                    &vi[2], &ui[2], &ni[2],
                    &vi[3], &ui[3], &ni[3]);

            MeshFace face1 = { vi[0] - 1, vi[1] - 1, vi[2] - 1,
                                ni[0] - 1, ni[1] - 1, ni[2] - 1,
                                ui[0] - 1, ui[1] - 1, ui[2] - 1};

            MeshFace face2 = { vi[0] - 1, vi[2] - 1, vi[3] - 1,
                                ni[0] - 1, ni[2] - 1, ni[3] - 1,
                                ui[0] - 1, ui[2] - 1, ui[3] - 1};


            m.faces.push_back(face1);
            m.faces.push_back(face2);
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

    return m;
}
