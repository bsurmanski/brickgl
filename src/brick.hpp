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

class Brick;
struct PegInfo {
    unsigned refcount;
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

    float buffer;
    float value;

    PegInfo *connected;

    PegInfo(Brick *own, PegType t, int n, int _x, int _y) {
        owner = own;
        type = t;
        typeId = n;
        x = _x;
        y = _y;
        value = 0.0f;
        connected = NULL;
        refcount = 1;
    }

    unsigned retain() {
        refcount++;
    }

    unsigned release() {
        refcount--;
        if(refcount <= 0) {
            delete this; // suicide!
            return 0;
        }

        return refcount;
    }

    void update() {
        switch(type) {
            case GROUND:
                buffer = 0.0f;
                break;
            case POWER:
                buffer = 1.0f;
                break;
            case INPUT:
                if(connected) buffer = connected->value;
                else buffer = 0.0f;
                break;
            case INOUT: //TODO: figure out them wires
            case OUTPUT:
                buffer = 0.0f;
                break;
        }
    }

    void setValue(float f) {
        // peg value is double buffered
        buffer = f;
    }

    float getValue() {
        return value;
    }

    bool isActive() {
        return value > 0.5f;
    }

    void flip() {
        value = buffer;
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
        BRICK_UNKNOWN,
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

    int npegs() { return length() * width(); }

    box getBox();
    box pegBox(int i, int j);

    virtual unsigned length() = 0;
    virtual unsigned width() = 0;
    virtual bool flat() { return false; }

    virtual void draw(DrawDevice *dev);
    virtual void light(DrawDevice *dev) {}
    virtual void update() {}
    virtual void flip() {}
    bool connect(Brick *o);
    bool connectPeg(int peg, Brick *o, int pego);
    void disconnect(Brick *o);
    virtual Brick *copy() = 0;
    virtual PegInfo *getPegInfo(int x, int y)=0;
    virtual std::string typeName()=0;

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

    Brick(vec4 position=vec4(0,0,0,1), vec4 rotation=vec4(0,0,0,0));
    Brick() {}
    virtual ~Brick() {}

    mat4 getMatrix();
    mat4 getPegMatrix(unsigned i, unsigned j);

    bool collides(Brick *b2);

    void rotate(vec4 r) { rotation = rotation + r; }
};

class TwoInputBrick : public Brick {
    protected:
    PegInfo *pegVcc;
    PegInfo *pegIn0;
    PegInfo *pegIn1;
    PegInfo *pegGnd;
    PegInfo *pegOut;

    public:
    TwoInputBrick(vec4 position=vec4(0,0,0,1), vec4 rotation=vec4(0,0,0,0))
        : Brick(position, rotation) {
            pegVcc = new PegInfo(this, PegInfo::POWER, 0, 0, 0);
            pegIn0 = new PegInfo(this, PegInfo::INPUT, 0, 1, 0);
            pegIn1 = new PegInfo(this, PegInfo::INPUT, 1, 2, 0);
            pegGnd = new PegInfo(this, PegInfo::GROUND, 0, 3, 0);
            pegOut = new PegInfo(this, PegInfo::OUTPUT, 0, 1, 1);
        }

    virtual ~TwoInputBrick() {
        pegVcc->release();
        pegIn0->release();
        pegIn1->release();
        pegGnd->release();
        pegOut->release();
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

    virtual void flip() {
        pegVcc->flip();
        pegGnd->flip();
        pegIn0->flip();
        pegIn1->flip();
        pegOut->flip();
    }
};

class ORBrick : public TwoInputBrick {
    public:
    ORBrick(vec4 pos=vec4(0,0,0,1), vec4 rot=vec4(0,0,0,0)) :
        TwoInputBrick(pos, rot)
    {}
    virtual bool flat() { return false; }
    Brick *copy() { return new ORBrick(position, rotation); }

    virtual void update() {
        pegVcc->setValue(1.0f);
        pegGnd->setValue(0.0f);
        pegIn0->update();
        pegIn1->update();
        pegOut->setValue(pegIn0->isActive() || pegIn1->isActive());
    }

    virtual std::string typeName() { return "or"; }

};

class ANDBrick : public TwoInputBrick {
    public:
    ANDBrick(vec4 pos=vec4(0,0,0,1), vec4 rot=vec4(0,0,0,0)) :
        TwoInputBrick(pos, rot)
    {}
    virtual bool flat() { return false; }
    Brick *copy() { return new ANDBrick(position, rotation); }

    virtual void update() {
        pegVcc->setValue(1.0f);
        pegGnd->setValue(0.0f);
        pegIn0->update();
        pegIn1->update();
        pegOut->setValue(pegIn0->isActive() && pegIn1->isActive());
    }

    virtual std::string typeName() { return "and"; }
};

class Wire8Brick : public Brick {
    PegInfo *pegWire[8];

    public:
    Wire8Brick(vec4 pos=vec4(0,0,0,1), vec4 rot=vec4(0,0,0,0)) : Brick(pos, rot) {
        for(int i = 0; i < 8; i++) {
            pegWire[i] = new PegInfo(this, PegInfo::INOUT, 0, i, 0);
        }
    }

    virtual ~Wire8Brick() {
        for(int i = 0; i < 8; i++)
            pegWire[i]->release();
    }

    virtual void update() {
        for(int i = 0; i < 8; i++) {
            pegWire[i]->update();
        }
    }
    virtual unsigned length() { return 8; }
    virtual unsigned width() { return 1; }
    virtual bool flat() { return false; }
    Brick *copy() { return new Wire8Brick(position, rotation); }
    virtual PegInfo *getPegInfo(int x, int y) {
        if(x < 0 || y < 0 || x >= length() || y >= width()) return NULL; // invalid
        return pegWire[x];
    }

    virtual void flip() {
        //TODO: set value of pegs
    }

    virtual std::string typeName() { return "wire"; }
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
        pegIn0->release();
        pegGnd->release();
    }

    virtual bool isActive() { return pegIn0->isActive(); }
    virtual void light(DrawDevice *dev);
    virtual void draw(DrawDevice *dev);
    virtual unsigned length() { return 2; }
    virtual unsigned width() { return 1; }

    Brick *copy() {
        return new LEDBrick(position, rotation);
    }

    virtual PegInfo *getPegInfo(int x, int y) {
        if(x < 0 || y < 0 || x >= length() || y >= width()) return NULL; // invalid
        if(x == 0) return pegIn0;
        if(x == 1) return pegGnd;
        return NULL;
    }

    virtual void update() {
        pegIn0->update();
        pegGnd->update();
    }

    virtual void flip() {
        pegIn0->flip();
        pegGnd->flip();
    }

    virtual std::string typeName() { return "led"; }
};

#endif
