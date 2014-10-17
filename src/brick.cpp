#include "brick.hpp"

#include "framework/draw/mesh.hpp"
#include "framework/draw/objFormat.hpp"
#include "framework/draw/pngFormat.hpp"

#define LEGO_UNIT 1.6
#define LEGO_STDHEIGHT (6*LEGO_UNIT)
#define LEGO_STDWIDTH (5*LEGO_UNIT)
#define LEGO_SHORTHEIGHT (2*LEGO_UNIT)
#define LEGO_PEGHEIGHT LEGO_UNIT
#define LEGO_HALFWIDTH 4.0

bool Brick::isInit = false;

GLMesh *Brick::fullMesh;
GLMesh *Brick::flatMesh;
GLTexture *Brick::plateTexture;
GLTexture *Brick::brickTexture;
GLTexture *Brick::groundTexture;
GLTexture *Brick::powerTexture;
GLTexture *Brick::outputTexture;
GLTexture *Brick::input1Texture;
GLTexture *Brick::input2Texture;

/*
bool Brick::Peg::collides(Brick *b)
{

}

bool Brick::Peg::connects(Peg *p)
{

}*/

box Brick::getBox()
{
    // offset by half a lego unit to align to pegs
    vec4 pos = getMatrix() * vec4(-LEGO_HALFWIDTH, 0, -LEGO_HALFWIDTH, 1);
    vec4 dim = getMatrix() * vec4(length() * LEGO_STDWIDTH,
                                  LEGO_STDHEIGHT,
                                  width() * LEGO_STDWIDTH, 0);


    // trim by 0.1 in each dimension to provide
    // a bit of buffer room for FP error and the like
    box b(pos, dim);
    b.trim(0.1f);
    return b;
}

box Brick::pegBox(int i, int j)
{
    vec4 pos = getPegMatrix(i, j) * vec4(-LEGO_HALFWIDTH, 0, -LEGO_HALFWIDTH, 1);
    vec4 dim = getPegMatrix(i, j) * vec4(LEGO_STDWIDTH, LEGO_STDHEIGHT + LEGO_PEGHEIGHT, LEGO_STDWIDTH, 0);
    box pbox(pos, dim);
    pbox.trim(0.1f);
    return pbox;
}

void Brick::init()
{
    if(!isInit)
    {
        isInit = true;
        Mesh nmesh = objLoad("res/1x1.obj");
        Mesh fmesh = objLoad("res/1x1f.obj");
        fullMesh = new GLMesh(nmesh);
        flatMesh = new GLMesh(fmesh);

        Image plateImage = pngLoad("res/grass.png");
        Image brickImage = pngLoad("res/pink2.png");
        Image input1Image = pngLoad("res/input1Texture.png");
        Image input2Image = pngLoad("res/input2Texture.png");
        Image outputImage = pngLoad("res/outputTexture.png");
        Image powerImage = pngLoad("res/powerTexture.png");
        Image groundImage = pngLoad("res/groundTexture.png");

        plateTexture = new GLTexture(plateImage);
        brickTexture = new GLTexture(brickImage);
        input1Texture = new GLTexture(input1Image);
        input2Texture = new GLTexture(input2Image);
        outputTexture = new GLTexture(outputImage);
        powerTexture = new GLTexture(powerImage);
        groundTexture = new GLTexture(groundImage);
    }

}


Brick::Brick(vec4 p, vec4 r) : position(p), rotation(r)
{
    init();
}

//TODO: rotation
bool Brick::collides(Brick *b2)
{
    box o = b2->getBox();
    return getBox().collides3(o);
}

bool Brick::is2Input()
{
    switch(type)
    {
        case BRICK_OR:
        case BRICK_AND:
            return true;
        default:
            return false;
    }
}

mat4 Brick::getMatrix()
{
    return mat4::getTranslation(position) *

        // XXX translation is to offset from peg to corner, so collision works right with rotation
            //mat4::getTranslation(vec4(-4.0f, 0, -4.0f, 0)) *
            mat4::getRotation(rotation);
            //mat4::getTranslation(vec4(4.0f, 0, 4.0f, 0));
}

mat4 Brick::getPegMatrix(unsigned i, unsigned j)
{
    return mat4::getTranslation(position) *
        mat4::getRotation(rotation) *
        mat4::getTranslation(vec4(i * 8, 0, j * 8, 0));
}

void Brick::draw(DrawDevice *dev)
{
    for(int j = 0; j < width(); j++)
    {
        for(int i = 0; i < length(); i++)
        {
            mat4 mMat =
                mat4::getTranslation(position) *
                mat4::getRotation(rotation) *
                mat4::getTranslation(vec4(i * 8, 0, j * 8, 1));
            ((GLDrawDevice*)dev)->drawMesh(flat() ? flatMesh : fullMesh,
                this->getTexture(i, j), mMat);
        }
    }

    //pegBox(0, 0).debugDraw(dev);
}

// when calling this function, we assume that the bricks are positioned
// adequately (the bricks are touching but not colliding, or not touching at all).
// If the bricks are intersecting, garbage connections will be made.
bool Brick::connect(Brick *o)
{
    // collect connections made so we can assert validity
    std::vector<std::pair<PegInfo*, PegInfo*>> connections;

    // 4 nested loops? O(n^4) is a dreadful algorithmic bound!
    // but, since we are working with such small numbers,
    // (bricks likely being 2*4 at largest)
    // and since this get called only once rarely, we should be fine.
    //
    // i/j iterator for self, x/y iterator for other
    for(int i = 0; i < length(); i++) {
        for(int j = 0; j < width(); j++) {
            for(int x = 0; x < o->length(); x++) {
                for(int y = 0; y < o->width(); y++) {
                    box obox = o->pegBox(x, y);
                    // these pegs are connected
                    if(pegBox(i, j).collides3(obox)) {
                        PegInfo *pi = getPegInfo(i, j);
                        PegInfo *pio = o->getPegInfo(x, y);
                        if(!pi->connect(pio) || !pio->connect(pi)) {
                            goto ERR;
                        }
                    }
                }
            }
        }
    }
    return true;

ERR:
    disconnect(o);
    o->disconnect(this);
    return false;
}

void Brick::disconnect(Brick *o) {
    int i = 0;
    PegInfo *pi;
    while((pi = getPegInfo(i++))) {
        if(pi->connected && pi->connected->owner == o) {
            pi->connected->disconnect();
            pi->disconnect();
        }
    }
}

void LEDBrick::draw(DrawDevice *dev)
{
    for(int j = 0; j < width(); j++)
    {
        for(int i = 0; i < length(); i++)
        {
            mat4 mMat =
                mat4::getTranslation(position) *
                mat4::getRotation(rotation) *
                mat4::getTranslation(vec4(i * 8, 0, j * 8, 1));
            if(isActive()) {
            ((GLDrawDevice*)dev)->drawMeshUnlit(flat() ? flatMesh : fullMesh,
                this->getTexture(i, j), mMat);
            } else {
            ((GLDrawDevice*)dev)->drawMesh(flat() ? flatMesh : fullMesh,
                this->getTexture(i, j), mMat);
            }
        }
    }
}

void LEDBrick::light(DrawDevice *dev) {
    if(isActive())
        ((GLDrawDevice*)dev)->drawLight(position + vec4(2,6,2), vec4(1,1,1), 0.4);
}
