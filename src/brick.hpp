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

    //Peg *pegs;

    virtual GLTexture *getTexture(int i, int j);


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
    //Brick(Brick &);
    Brick() : value(0.0f) {}
    //~Brick() { if(pegs) delete[] pegs; }

    mat4 getMatrix();
    mat4 getPegMatrix(unsigned i, unsigned j);

    //XXX dimensions should be 8x8x9.6, trimmed for allow space in collision detection
    float left() { return position.x; }
    float right() { return position.x + length() * 7.900f; }
    float front() { return position.z - width() * 7.900f; } //TODO: check front vs back
    float back() { return position.z; }
    float top() { return position.y + 9.500f; } // TODO: check virtical size
    float bottom() { return position.y; }
    bool collides(Brick *b2);

    void rotate(vec4 r) { rotation = rotation + r; }
};

class TwoInputBrick : public Brick {
    Brick *input1;
    Brick *input2;
    public:
    TwoInputBrick(vec4 position=vec4(0,0,0,1), vec4 rotation=vec4(0,0,0,0), float value=0.0f)
        : Brick(position, rotation, value), input1(0), input2(0) {}

    virtual GLTexture *getTexture(int i, int j)
    {
        if(i == 0) return powerTexture;
        if(i == 3) return groundTexture;
        if(j == 1) return outputTexture;
        if(i == 1) return input1Texture;
        return input2Texture;
    }
};

class ORBrick : public TwoInputBrick {
    public:
    ORBrick(vec4 pos=vec4(0,0,0,1), vec4 rot=vec4(0,0,0,0)) :
        TwoInputBrick(pos, rot)
    {}
    virtual void update() {}
    virtual unsigned length() { return 4; }
    virtual unsigned width() { return 2; }
    virtual bool flat() { return false; }
    Brick *copy() { return new ORBrick(position, rotation); }
};

class ANDBrick : public TwoInputBrick {
    public:
    ANDBrick(vec4 pos=vec4(0,0,0,1), vec4 rot=vec4(0,0,0,0)) :
        TwoInputBrick(pos, rot)
    {}
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

    virtual GLTexture *getTexture(int i, int j)
    {
        return groundTexture;
    }
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

    virtual GLTexture *getTexture(int i, int j)
    {
        if(i == 0) return powerTexture;
        if(i == 1) return groundTexture;
        return NULL;
    }
};

#endif
