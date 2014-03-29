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

struct Brick
{
    private:
    static bool isInit;
    static std::vector<Brick> brickTypes;

    struct Peg
    {
        Peg *top;
        Peg *bottom;
        Brick *owner;

        // pegs are automata, have buffered value
        float value;
        float back;

        void rupdate() {
            int n = 1 + !!top + !!bottom;
            back = ((top ? top->value : 0) + (bottom ? bottom->value : 0) + value) / (float) n;

            if(top && !top->owner->tagged) top->owner->rupdate();
            if(bottom && !bottom->owner->tagged) bottom->owner->rupdate();
        }

        void rflip() {
            value = back;
            if(top && top->owner->tagged) top->rflip();
            if(bottom && bottom->owner->tagged) top->rflip();
        }

        Peg() : owner(0), top(0), bottom(0), value(0) {}
        Peg(Brick *o) : owner(o), top(0), bottom(0), value(0) {}
    };

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

    bool tagged; // brick has been visited
    Peg *pegs;

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

    int npegs() { return length() * width(); }

    box getBox();
    box pegBox(int i, int j);

    void draw(DrawDevice *dev);
    void rupdate();
    void rflip();
    bool connect(Brick *o);

    Brick(Type type, vec4 position);
    Brick() : pegs(0) {}
    //~Brick() { if(pegs) delete[] pegs; }

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
