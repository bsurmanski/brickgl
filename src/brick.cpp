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
            //return true;
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

Brick::Brick(Type t, vec4 p) : position(p), type(t)
{
    position = vec4(0,0,0,1);
    rotation = vec4(0,0,0,0);
    init();

    connections = new std::pair<Brick*, float>[length() * width()];
    for(int i = 0; i < length() * width(); i++)
    {
        connections[i] = std::pair<Brick*, float>(NULL, 0.0f);
    }
}

//TODO: rotation
bool Brick::collides(Brick &b2)
{
    return
        (right() > b2.left() && left() < b2.right()) && // x is intersecting
        (back() > b2.front() && front() < b2.back()) &&
        (bottom() < b2.top() && top() > b2.bottom());
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
    return groundTexture; //TODO other textures
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
}
