/**
 * brick.h
 * brickgl
 * January 21, 2014
 * Brandon Surmanski
 */

#include "vector.hpp"
#include "framework/draw/glMesh.hpp"
#include "framework/draw/glTexture.hpp"

#include <vector>

struct Brick
{
    static bool isInit;
    static std::vector<Brick> brickTypes;

    static GLMesh *fullMesh;
    static GLMesh *flatMesh;
    static GLTexture *plateTexture;
    static GLTexture *brickTexture;

    static void init();

    vec4 position;
    unsigned w;
    unsigned h;
    bool flat;
    Brick(vec4 p, unsigned wi, unsigned hi, bool fl = false);

    float left() { return position.x; }
    float right() { return position.x + w * 7.99f; }
    float front() { return position.z - h * 7.99f; } //TODO: check front vs back
    float back() { return position.z; }
    float top() { return position.y + 9.599f; } // TODO: check virtical size
    float bottom() { return position.y; }
    bool collides(Brick &b2);
};
