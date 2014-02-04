#include "glMesh.hpp"

#include <stdint.h>
#include <string.h>
#include <limits.h>
#include <math.h>

struct IVertex
{
    unsigned v;
    unsigned u;
    unsigned n;
};

struct GLVertex
{
    float v[3]; 
    int16_t norm[3];
    uint16_t uv[2];
    uint8_t PADDING[2];
};

struct GLIndex
{
    uint16_t v[3];
};

uint16_t float_to_ushort(float f)
{
    if(f >= 1.0f) return USHRT_MAX;
    if(f < 0.0f) return 0;
    return floor(f * USHRT_MAX);
}

int16_t float_to_short(float f)
{
    if(f >= 1.0f) return SHRT_MAX;
    if(f <= -1.0f) return SHRT_MIN;
    return round(f * SHRT_MAX);
}

void float_to_short3(float fv[3], int16_t sv[3])
{
    sv[0] = float_to_short(fv[0]);
    sv[1] = float_to_short(fv[1]);
    sv[2] = float_to_short(fv[2]);
}

void float_to_ushort3(float fv[3], uint16_t usv[3])
{
    usv[0] = float_to_ushort(fv[0]);
    usv[1] = float_to_ushort(fv[1]);
    usv[2] = float_to_ushort(fv[2]);
}

void float_to_short2(float fv[2], int16_t sv[2])
{
    sv[0] = float_to_short(fv[0]);
    sv[1] = float_to_short(fv[1]);
}

void float_to_ushort2(float fv[2], uint16_t usv[2])
{
    usv[0] = float_to_ushort(fv[0]);
    usv[1] = float_to_ushort(fv[1]);
}

GLMesh::GLMesh(Mesh &m)
{
    glGenBuffers(1, &ibuffer);
    glGenBuffers(1, &vbuffer);

    // resolve GLVertex / GLIndex from mesh

    std::vector<GLIndex> glindex;
    std::vector<IVertex> iverts;

    int vindex;
    for(int i = 0; i < m.faces.size(); i++)
    {
        GLIndex gli;
        for(int j = 0; j < 3; j++)
        {
            IVertex iv = { m.faces[i].v[j], m.faces[i].n[j], m.faces[i].uv[j] };
            vindex = -1;

            // look through list, see if equiv IVert already there
            // if not, insert an entry
            for(int k = 0; k < iverts.size(); k++)
            {
                if(!memcmp(&iv, &iverts[k], sizeof(IVertex)))
                {
                    vindex = k;
                    break;
                }
            }

            // equiv IVertex not found. insert entry
            if(vindex == -1)
            {
                vindex = iverts.size();
                iverts.push_back(iv); 
            }

            gli.v[j] = vindex;       
        }
        glindex.push_back(gli);
    }

    std::vector<GLVertex> glverts;

    for(int i = 0; i < iverts.size(); i++)
    {
        float *vptr = m.verts[iverts[i].v].v;
        float *nptr = m.norms[iverts[i].n].v;
        float *uptr = m.uvs[iverts[i].u].v;
        int16_t inptr[3];
        uint16_t iuptr[2];
        float_to_short3(nptr, inptr);
        float_to_ushort2(uptr, iuptr);
        GLVertex glvert = { vptr[0], vptr[1], vptr[2],
                            inptr[0], inptr[1], inptr[2],
                            iuptr[0], iuptr[1]};
        glverts.push_back(glvert);
    }


    glBindBuffer(GL_ARRAY_BUFFER, vbuffer);
    glBufferData(GL_ARRAY_BUFFER, 
            sizeof(GLVertex) * glverts.size(), 
            glverts.data(),
            GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 
            sizeof(GLIndex) * glindex.size(), 
            glindex.data(),
            GL_STATIC_DRAW);

    nelem = glindex.size() * 3;
}
