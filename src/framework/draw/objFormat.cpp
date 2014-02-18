#include "objFormat.hpp"
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

Mesh objLoad(std::string filenm)
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
            uint16_t vi[3];
            uint16_t ni[3];
            uint16_t ui[3];

            fscanf(file, "f %hu/%hu/%hu %hu/%hu/%hu %hu/%hu/%hu",
                    &vi[0], &ui[0], &ni[0],
                    &vi[1], &ui[1], &ni[1],
                    &vi[2], &ui[2], &ni[2]);

            MeshFace face = { vi[0] - 1, vi[1] - 1, vi[2] - 1,
                                ni[0] - 1, ni[1] - 1, ni[2] - 1,
                                ui[0] - 1, ui[1] - 1, ui[2] - 1};

            m.faces.push_back(face);
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
