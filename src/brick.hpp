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

class Brick;
struct PegInfo {
    enum PegType {
        INPUT,
        OUTPUT,
        INOUT,
        POWER,
        GROUND
    };

    Brick *owner;
    PegType type;
    int typeId; //eg, input0, input1, input2
    int x;
    int y;

    PegInfo *connected;

    PegInfo(Brick *own, PegType t, int n, int _x, int _y) {
        owner = own;
        type = t;
        typeId = n;
        x = _x;
        y = _y;
        connected = NULL;
    }

    bool connect(PegInfo *o) {
        if(owner == o->owner) return false; //cannot connect to self
        if(connected) return false; // peg already connected
        if(type == INPUT && o->type == INPUT) return false; // input cannot connect to input
        if(type == OUTPUT && o->type == OUTPUT) return false; //output cannot connect to output
        connected = o;
        return true;
    }

    void disconnect() {
        connected = NULL;
    }
};

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

    virtual GLTexture *getTexture(int i, int j) {
        PegInfo *pi = getPegInfo(i, j);
        if(!pi) return groundTexture;
        switch(pi->type) {
            case PegInfo::INPUT: return input1Texture;
            case PegInfo::OUTPUT: return outputTexture;
            case PegInfo::INOUT: return groundTexture;
            case PegInfo::POWER: return powerTexture;
            case PegInfo::GROUND: return groundTexture;
        }

        return groundTexture; //ERROR, somethings wrong
    }


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
    bool connectPeg(int peg, Brick *o, int pego);
    void disconnect(Brick *o);
    virtual bool isActive() { return value > 0.1f; }
    virtual Brick *copy() = 0;
    virtual PegInfo *getPegInfo(int x, int y)=0;

    PegInfo *getPegInfo(int n) {
        return getPegInfo(n % width(), n / width());
    }

    virtual PegInfo *getTypedPeg(PegInfo::PegType t, int n) {
        int c = 0;
        for(int i = 0; i < width(); i++) {
            for(int j = 0; j < length(); j++) {
                PegInfo *pi = getPegInfo(i, j);
                if(pi->type == t) {
                    if(c == n) return pi;
                    c++;
                }
            }
        }
        return NULL;
    }

    PegInfo *getInput(int n) {
        return getTypedPeg(PegInfo::INPUT, n);
    }

    PegInfo *getOutput(int n) {
        return getTypedPeg(PegInfo::OUTPUT, n);
    }

    PegInfo *getGround(int n) {
        return getTypedPeg(PegInfo::GROUND, n);
    }

    PegInfo *getPower(int n) {
        return getTypedPeg(PegInfo::POWER, n);
    }

    Brick(vec4 position=vec4(0,0,0,1), vec4 rotation=vec4(0,0,0,0), float value=0.0f);
    Brick() : value(0.0f) {}
    virtual ~Brick() {}

    mat4 getMatrix();
    mat4 getPegMatrix(unsigned i, unsigned j);

    bool collides(Brick *b2);

    void rotate(vec4 r) { rotation = rotation + r; }
};

class TwoInputBrick : public Brick {
    PegInfo *pegVcc;
    PegInfo *pegIn0;
    PegInfo *pegIn1;
    PegInfo *pegGnd;
    PegInfo *pegOut;

    public:
    TwoInputBrick(vec4 position=vec4(0,0,0,1), vec4 rotation=vec4(0,0,0,0), float value=0.0f)
        : Brick(position, rotation, value) {
            pegVcc = new PegInfo(this, PegInfo::POWER, 0, 0, 0);
            pegIn0 = new PegInfo(this, PegInfo::INPUT, 0, 1, 0);
            pegIn1 = new PegInfo(this, PegInfo::INPUT, 1, 2, 0);
            pegGnd = new PegInfo(this, PegInfo::GROUND, 0, 3, 0);
            pegOut = new PegInfo(this, PegInfo::OUTPUT, 0, 1, 1);
        }

    virtual ~TwoInputBrick() {
        delete pegVcc;
        delete pegIn0;
        delete pegIn1;
        delete pegGnd;
        delete pegOut;
    }

    virtual unsigned length() { return 4; }
    virtual unsigned width() { return 2; }

    virtual PegInfo *getPegInfo(int x, int y) {
        if(x < 0 || y < 0 || x >= length() || y >= width()) return NULL; // invalid
        if(x == 0) return pegVcc;
        if(x == 3) return pegGnd;
        if(y == 0) {
            if(x == 1) {
                return pegIn0;
            }
            return pegIn1;
        } else if(y == 1) {
            return pegOut;
        }
        return NULL;
    }
};

class ORBrick : public TwoInputBrick {
    public:
    ORBrick(vec4 pos=vec4(0,0,0,1), vec4 rot=vec4(0,0,0,0)) :
        TwoInputBrick(pos, rot)
    {}
    virtual void update() {}
    virtual bool flat() { return false; }
    Brick *copy() { return new ORBrick(position, rotation); }
};

class ANDBrick : public TwoInputBrick {
    public:
    ANDBrick(vec4 pos=vec4(0,0,0,1), vec4 rot=vec4(0,0,0,0)) :
        TwoInputBrick(pos, rot)
    {}
    virtual void update() {}
    virtual bool flat() { return false; }
    Brick *copy() { return new ANDBrick(position, rotation); }
};

class Wire8Brick : public Brick {
    PegInfo *pegWire;

    public:
    Wire8Brick(vec4 pos=vec4(0,0,0,1), vec4 rot=vec4(0,0,0,0)) : Brick(pos, rot) {
        pegWire = new PegInfo(this, PegInfo::INOUT, 0, 0, 0);
    }

    virtual ~Wire8Brick() {
        delete pegWire;
    }

    virtual void update() {}
    virtual unsigned length() { return 8; }
    virtual unsigned width() { return 1; }
    virtual bool flat() { return false; }
    Brick *copy() { return new Wire8Brick(position, rotation); }
    virtual PegInfo *getPegInfo(int x, int y) {
        if(x < 0 || y < 0 || x >= length() || y >= width()) return NULL; // invalid
        return pegWire;
    }

};

class LEDBrick : public Brick {
    PegInfo *pegIn0;
    PegInfo *pegGnd;

    public:
    LEDBrick(vec4 pos=vec4(0,0,0,1), vec4 rot=vec4(0,0,0,0)) : Brick(pos, rot) {
        pegIn0 = new PegInfo(this, PegInfo::INPUT, 0, 0, 0);
        pegGnd = new PegInfo(this, PegInfo::GROUND, 0, 1, 0);
    }

    virtual ~LEDBrick() {
        delete pegIn0;
        delete pegGnd;
    }

    virtual bool isActive() { return value > 0.1f; }
    virtual void update() {}
    virtual void light(DrawDevice *dev);
    virtual void draw(DrawDevice *dev);
    virtual unsigned length() { return 2; }
    virtual unsigned width() { return 1; }
    Brick *copy() { return new LEDBrick(position, rotation); }
    virtual PegInfo *getPegInfo(int x, int y) {
        if(x < 0 || y < 0 || x >= length() || y >= width()) return NULL; // invalid
        if(x == 0) return pegIn0;
        if(x == 1) return pegGnd;
        return NULL;
    }

};

#endif
