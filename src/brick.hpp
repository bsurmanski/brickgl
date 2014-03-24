/**
 * brick.h
 * brickgl
 * January 21, 2014
 * Brandon Surmanski
 */

#ifndef _BRICK_HPP
#define _BRICK_HPP

#include "vector.hpp"
#include "box.hpp"
#include "framework/draw/glMesh.hpp"
#include "framework/draw/glTexture.hpp"
#include "framework/draw/drawDevice.hpp"
#include "framework/draw/glDrawDevice.hpp"

#include <vector>
#include <array>
//#include <pair>

struct Brick
{
    private:
    static bool isInit;
    static std::vector<Brick> brickTypes;

    public:
    static GLMesh *fullMesh;
    static GLMesh *flatMesh;
    static GLTexture *plateTexture;
    static GLTexture *brickTexture;
    static GLTexture *groundTexture;
    static GLTexture *powerTexture;
    static GLTexture *outputTexture;
    static GLTexture *input1Texture;
    static GLTexture *input2Texture;

    static void init();

    std::pair<Brick *, float>* connections;

    GLTexture *getTexture(int i, int j);


    enum Type
    {
        BRICK_OR,
        BRICK_AND,
        BRICK_LED,
        BRICK_WIRE8,
        BRICK_POWER,
        BRICK_PLATE32,
        BRICK_PLATE2x4
    };

    bool is2Input();

    Type type;

    vec4 position;
    vec4 rotation;

    unsigned length();
    unsigned width();
    bool flat();

    box getBox();

    void draw(DrawDevice *dev);

    Brick(Type type, vec4 position);
    Brick(){}

    mat4 getMatrix();
    mat4 getPegMatrix(unsigned i, unsigned j);

    float left() { return position.x; }
    float right() { return position.x + length() * 7.99f; }
    float front() { return position.z - width() * 7.99f; } //TODO: check front vs back
    float back() { return position.z; }
    float top() { return position.y + 9.599f; } // TODO: check virtical size
    float bottom() { return position.y; }
    bool collides(Brick &b2);

    void rotate(vec4 r) { rotation = rotation + r; }
};

#endif
