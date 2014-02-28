
#include "drawDevice.hpp"

DrawDevice::DrawDevice()
{
    mMatrix = mat4::getIdentity();
    vMatrix = mat4::getIdentity();
    float x = 1.0f;
    float y = 0.75f;
    float fovmod = 0.8;
    x *= fovmod;
    y *= fovmod;
    pMatrix = mat4::getFrustum(-x, x, -y, y, 1.0f, 512.0f);
}

DrawDevice::~DrawDevice()
{

}

void DrawDevice::update(float dt)
{

}
