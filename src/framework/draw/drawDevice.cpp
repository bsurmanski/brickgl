
#include "drawDevice.hpp"

DrawDevice::DrawDevice()
{
    mMatrix = mat4::getIdentity();
    vMatrix = mat4::getIdentity();
    pMatrix = mat4::getFrustum(-1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 512.0f);
}

DrawDevice::~DrawDevice()
{

}

void DrawDevice::update(float dt)
{

}
