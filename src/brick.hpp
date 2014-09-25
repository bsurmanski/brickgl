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

class Brick
{
    private:
    static bool isInit;
    static std::vector<Brick> brickTypes;

    struct Peg
    {
        Peg *top;
        Peg *bottom;
        Brick *owner;

        int i;
        int j;

        // pegs are automata, have buffered value
        float value;
        float back;

        bool collides(Brick *b);
        bool connects(Peg *p);

        void update() {
            int n = 1 + !!top + !!bottom;
            back = ((top ? top->value : 0) + (bottom ? bottom->value : 0) + value) / (float) n;
        }

        void flip() {
            value = back;
        }

        Peg() : owner(0), top(0), bottom(0), value(0) {}
        Peg(Brick *o, int x, int y) : owner(o), top(0), bottom(0), value(0), i(x), j(y)  {}
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

    float value;
    bool is2Input();

    Type type;

    vec4 position;
    vec4 rotation;


    int npegs() { return length() * width(); }

    box getBox();
    box pegBox(int i, int j);

    virtual unsigned length() = 0;
    virtual unsigned width() = 0;
    virtual bool flat() { return false; }

    virtual void draw(DrawDevice *dev);
    virtual void light(DrawDevice *dev) {}
    virtual void update() {}
    virtual void flip();
    bool connect(Brick *o);
    virtual bool isActive() { return value > 0.1f; }
    virtual Brick *copy() = 0;

    Brick(vec4 position=vec4(0,0,0,1), vec4 rotation=vec4(0,0,0,0), float value=0.0f);
    Brick(Brick &);
    Brick() : pegs(0), value(0.0f) {}
    //~Brick() { if(pegs) delete[] pegs; }

    mat4 getMatrix();
    mat4 getPegMatrix(unsigned i, unsigned j);

    float left() { return position.x; }
    float right() { return position.x + length() * 7.99f; }
    float front() { return position.z - width() * 7.99f; } //TODO: check front vs back
    float back() { return position.z; }
    float top() { return position.y + 9.599f; } // TODO: check virtical size
    float bottom() { return position.y; }
    bool collides(Brick *b2);

    void rotate(vec4 r) { rotation = rotation + r; }
};

class ORBrick : public Brick {
    public:
    ORBrick(vec4 pos=vec4(0,0,0,1), vec4 rot=vec4(0,0,0,0)) : Brick(pos, rot) {}
    virtual void update() {}
    virtual unsigned length() { return 4; }
    virtual unsigned width() { return 2; }
    virtual bool flat() { return false; }
    Brick *copy() { return new ORBrick(position, rotation); }
};

class ANDBrick : public Brick {
    public:
    ANDBrick(vec4 pos=vec4(0,0,0,1), vec4 rot=vec4(0,0,0,0)) : Brick(pos, rot) {}
    virtual void update() {}
    virtual unsigned length() { return 4; }
    virtual unsigned width() { return 2; }
    virtual bool flat() { return false; }
    Brick *copy() { return new ANDBrick(position, rotation); }
};

class Wire8Brick : public Brick {
    public:
    Wire8Brick(vec4 pos=vec4(0,0,0,1), vec4 rot=vec4(0,0,0,0)) : Brick(pos, rot) {}
    virtual void update() {}
    virtual unsigned length() { return 8; }
    virtual unsigned width() { return 1; }
    virtual bool flat() { return false; }
    Brick *copy() { return new Wire8Brick(position, rotation); }
};

class LEDBrick : public Brick {
    public:
    LEDBrick(vec4 pos=vec4(0,0,0,1), vec4 rot=vec4(0,0,0,0)) : Brick(pos, rot) {}
    virtual bool isActive() { return value > 0.1f; }
    virtual void update() {}
    virtual void light(DrawDevice *dev);
    virtual void draw(DrawDevice *dev);
    virtual unsigned length() { return 2; }
    virtual unsigned width() { return 1; }
    Brick *copy() { return new LEDBrick(position, rotation); }
};

#endif
