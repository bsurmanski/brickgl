/**
 * brick.h
 * brickgl
 * January 21, 2014
 * Brandon Surmanski
 */

#include "vector.hpp"
#include "framework/draw/glMesh.hpp"

struct Brick
{
    GLMesh *mesh;
    vec4 position;
    unsigned w;
    unsigned h;
    Brick(GLMesh *m, vec4 p, unsigned wi, unsigned hi) : mesh(m), position(p), w(wi), h(hi) {}
};

