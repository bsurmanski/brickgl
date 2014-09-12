#include "brick.hpp"

#include "framework/draw/mesh.hpp"
#include "framework/draw/objFormat.hpp"
#include "framework/draw/pngFormat.hpp"

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

bool Brick::Peg::collides(Brick *b)
{

}

bool Brick::Peg::connects(Peg *p)
{

}

unsigned Brick::width()
{
    switch(type)
    {
        case BRICK_AND:
        case BRICK_OR:
        case BRICK_PLATE2x4:
            return 2;
        case BRICK_LED:
            return 1;
        case BRICK_WIRE8:
            return 1;
        case BRICK_PLATE32:
            return 32;
        default: return 1;
    }
}

unsigned Brick::length()
{
    switch(type)
    {
        case BRICK_AND:
        case BRICK_OR:
        case BRICK_PLATE2x4:
            return 4;
        case BRICK_LED:
            return 2;
        case BRICK_WIRE8:
            return 8;
        case BRICK_PLATE32:
            return 32;
        default: return 1;
    }
}

bool Brick::flat()
{
    switch(type)
    {
        case BRICK_PLATE2x4:
        case BRICK_PLATE32:
            return true;

        case BRICK_WIRE8:
            //return true;
        default:
            return false;
    }
}

box Brick::getBox()
{
    vec4 pos = getMatrix() * vec4(-4.0,0,-4.0,1);
    vec4 dim = getMatrix() * vec4(length() * 7.99f, 9.599f, width() * 7.99f, 0);
    return box(pos, dim);
}

box Brick::pegBox(int i, int j)
{
    vec4 pos = getPegMatrix(i, j) * vec4(-4.0,0,-4.0,1);
    vec4 dim = getPegMatrix(i, j) * vec4(7.99f, 9.599f, 7.99f, 0);
    return box(pos, dim);
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

Brick::Brick(Type t, vec4 p) : position(p), type(t), tagged(false), value(0.0f)
{
    position = vec4(0,0,0,1);
    rotation = vec4(0,0,0,0);
    init();

    pegs = new Peg[npegs()];
    for(int i = 0; i < width(); i++)
    {
        for(int j = 0; j < length(); j++)
        {
            pegs[i * length() + j] = Peg(this, i, j);
        }
    }
    //for(int i = 0; i < npegs(); i++) pegs[i] = Peg(this);
}

Brick::Brick(Brick &oth) :
    position(oth.position),
    rotation(oth.rotation),
    type(oth.type),
    tagged(oth.tagged),
    value(oth.value)
{

}

//TODO: rotation
bool Brick::collides(Brick *b2)
{
    box o = b2->getBox();
    getBox().print();
    o.print();
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

GLTexture *Brick::getTexture(int i, int j)
{
    if(is2Input())
    {
        if(i == 0) return powerTexture;
        if(i == 3) return groundTexture;
        if(j == 1) return outputTexture;
        if(i == 1) return input1Texture;
        return input2Texture;
    }
    if(type == BRICK_LED)
    {
        if(i == 0) return powerTexture;
        if(i == 1) return groundTexture;
    }
    return groundTexture; //TODO other textures
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
            if(type == BRICK_LED && isActive()) {
            ((GLDrawDevice*)dev)->drawMeshUnlit(flat() ? flatMesh : fullMesh,
                this->getTexture(i, j), mMat);
            } else {
            ((GLDrawDevice*)dev)->drawMesh(flat() ? flatMesh : fullMesh,
                this->getTexture(i, j), mMat);
            }
        }
    }
}

void Brick::light(DrawDevice *dev)
{
    if(type == BRICK_LED && isActive())
        ((GLDrawDevice*)dev)->drawLight(position + vec4(2,6,2), vec4(1,1,1), 0.4);
}

void Brick::rupdate()
{
    tagged = true; // tag to prevent loops
    for(int i = 0; i < npegs(); i++)
    {
        pegs[i].rupdate(); // will update all connected pegs
    }
}

void Brick::rflip()
{
    tagged = false;
    for(int i = 0; i < npegs(); i++)
    {
        pegs[i].rflip(); // will flip all connected pegs
    }
}

bool Brick::connect(Brick *o)
{
    return false;
}
