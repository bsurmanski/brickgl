#include "brick.hpp"

#include "framework/draw/mesh.hpp"
#include "framework/draw/objFormat.hpp"
#include "framework/draw/pngFormat.hpp"

bool Brick::isInit = false;

GLMesh *Brick::fullMesh;
GLMesh *Brick::flatMesh;
GLTexture *Brick::plateTexture;
GLTexture *Brick::brickTexture;

unsigned Brick::width()
{
    switch(type)
    {
        case BRICK_AND:
        case BRICK_OR:
            return 2;
        case BRICK_LED:
            return 2;

        case BRICK_WIRE8:
            return 1;
        default: return 1;
    }
}

unsigned Brick::length()
{
    switch(type)
    {
        case BRICK_AND:
        case BRICK_OR:
            return 4;
        case BRICK_LED:
            return 2;
        case BRICK_WIRE8:
            return 8;
        default: return 1;
    }
}

bool Brick::flat()
{
    switch(type)
    {
        case BRICK_WIRE8:
            return true;
        default:
            return false;
    }
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
        plateTexture = new GLTexture(plateImage);
        brickTexture = new GLTexture(brickImage);
    }

}

Brick::Brick(Type t, vec4 p) : position(p), type(t)
{
    init();
}

bool Brick::collides(Brick &b2)
{
    return
        (right() > b2.left() && left() < b2.right()) && // x is intersecting
        (back() > b2.front() && front() < b2.back()) &&
        (bottom() < b2.top() && top() > b2.bottom());
}

void Brick::draw()
{

}
