#include "glMesh.hpp"

#include <stdint.h>
#include <string.h>
#include <limits.h>
#include <math.h>

GLMesh *GLMesh::cubeMesh = NULL;
GLMesh *GLMesh::quadMesh = NULL;

struct IVertex
{
    unsigned v;
    unsigned n;
    unsigned u;
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

GLMesh::~GLMesh()
{
    glDeleteBuffers(1, &ibuffer);
    glDeleteBuffers(1, &vbuffer);
}

GLMesh::GLMesh(GLVertex *v, int nv, GLIndex *i, int ni) {
    glGenBuffers(1, &ibuffer);
    glGenBuffers(1, &vbuffer);

    glBindBuffer(GL_ARRAY_BUFFER, vbuffer);
    glBufferData(GL_ARRAY_BUFFER,
            sizeof(GLVertex) * nv,
            v,
            GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
            sizeof(GLIndex) * ni,
            i,
            GL_STATIC_DRAW);

    nelem = ni * 3;
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

#define P INT16_MAX
#define N INT16_MIN
static GLVertex unitQuadV[4] = {
    //+Z
    {{ 1, -1,  1}, {0, 0, P}, {0, 0}, 0},
    {{ 1,  1,  1}, {0, 0, P}, {0, 0}, 0},
    {{-1, -1,  1}, {0, 0, P}, {0, 0}, 0},
    {{-1,  1,  1}, {0, 0, P}, {0, 0}, 0},
};

static GLVertex unitCubeV[32] = {
    //+X
    {{ 1, -1, -1}, { P, 0, 0}, {0, 0}, 0},
    {{ 1,  1, -1}, { P, 0, 0}, {0, 0}, 0},
    {{ 1,  1,  1}, { P, 0, 0}, {0, 0}, 0},
    {{ 1, -1,  1}, { P, 0, 0}, {0, 0}, 0},

    //-X
    {{-1, -1,  1}, { N, 0, 0}, {0, 0}, 0},
    {{-1,  1,  1}, { N, 0, 0}, {0, 0}, 0},
    {{-1,  1, -1}, { N, 0, 0}, {0, 0}, 0},
    {{-1, -1, -1}, { N, 0, 0}, {0, 0}, 0},

    //+Y
    {{ 1,  1, -1}, {0, P, 0}, {0, 0}, 0},
    {{-1,  1, -1}, {0, P, 0}, {0, 0}, 0},
    {{-1,  1,  1}, {0, P, 0}, {0, 0}, 0},
    {{ 1,  1,  1}, {0, P, 0}, {0, 0}, 0},

    //-Y
    {{-1, -1, -1}, {0, N, 0}, {0, 0}, 0},
    {{ 1, -1, -1}, {0, N, 0}, {0, 0}, 0},
    {{ 1, -1,  1}, {0, N, 0}, {0, 0}, 0},
    {{-1, -1,  1}, {0, N, 0}, {0, 0}, 0},

    //+Z
    {{-1, -1,  1}, {0, 0, P}, {0, 0}, 0},
    {{ 1, -1,  1}, {0, 0, P}, {0, 0}, 0},
    {{ 1,  1,  1}, {0, 0, P}, {0, 0}, 0},
    {{-1,  1,  1}, {0, 0, P}, {0, 0}, 0},

    //-Z
    {{-1, -1, -1}, {0, 0, N}, {0, 0}, 0},
    {{ 1, -1, -1}, {0, 0, N}, {0, 0}, 0},
    {{ 1,  1, -1}, {0, 0, N}, {0, 0}, 0},
    {{-1,  1, -1}, {0, 0, N}, {0, 0}, 0},
};
#undef P
#undef N

static GLIndex unitQuadI[2] = {
    {0, 1, 2}, {0, 2, 3},
};

static GLIndex unitCubeI[12] = {
    {0, 1, 2}, {0, 2, 3},
    {4, 5, 6}, {4, 6, 7},
    {8, 9, 10}, {8, 10, 11},
    {12, 13, 14}, {12, 14, 15},
    {16, 17, 18}, {16, 18, 19},
    {20, 21, 22}, {20, 22, 23}
};

#include <stdio.h>

GLMesh *GLMesh::getUnitCube() {
    if(!cubeMesh) {
        cubeMesh = new GLMesh(unitCubeV, 32, unitCubeI, 12);
    }

    return cubeMesh;
}

GLMesh *GLMesh::getUnitQuad() {
    if(!quadMesh) {
        quadMesh = new GLMesh(unitQuadV, 4, unitQuadI, 2);
    }

    return quadMesh;
}
