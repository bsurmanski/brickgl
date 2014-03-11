#include "brick.hpp"

#include "framework/draw/mesh.hpp"
#include "framework/draw/objFormat.hpp"
#include "framework/draw/pngFormat.hpp"

bool Brick::isInit = false;

GLMesh *Brick::fullMesh;
GLMesh *Brick::flatMesh;
GLTexture *Brick::plateTexture;
GLTexture *Brick::brickTexture;


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

Brick::Brick(vec4 p, unsigned wi, unsigned hi, bool fl) : position(p), w(wi), h(hi), flat(fl)
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
