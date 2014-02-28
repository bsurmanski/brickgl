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

    float left() { return position.x; }
    float right() { return position.x + w * 7.99f; }
    float front() { return position.z - h * 7.99f; } //TODO: check front vs back
    float back() { return position.z; }
    float top() { return position.y + 9.599f; } // TODO: check virtical size
    float bottom() { return position.y; }
    bool collides(Brick &b2);
};
