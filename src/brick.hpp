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

    enum Type
    {
        BRICK_OR,
        BRICK_AND,
        BRICK_LED,
        BRICK_WIRE8,
        BRICK_POWER
    };

    Type type;

    static void init();

    vec4 position;

    unsigned length();
    unsigned width();
    bool flat();

    void draw();

    Brick(Type type, vec4 position);

    float left() { return position.x; }
    float right() { return position.x + length() * 7.99f; }
    float front() { return position.z - width() * 7.99f; } //TODO: check front vs back
    float back() { return position.z; }
    float top() { return position.y + 9.599f; } // TODO: check virtical size
    float bottom() { return position.y; }
    bool collides(Brick &b2);
};
