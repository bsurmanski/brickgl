#include "camera.hpp"


Camera::Camera() : viewDirty(true)
{
    float x = 1.0f;
    float y = 0.75f;
    float fovmod = 0.8;
    x *= fovmod;
    y *= fovmod;
    perspective = mat4::getFrustum(-x, x, -y, y, 1.0f, 512.0f);
    position = vec4(0,0,0,1);
    rotation = vec4(-0.707, 0, 0, 0);
    offset = vec4(0, 0, -100, 1);
}

void Camera::calcView()
{
    view = mat4::getTranslation(offset) *
        mat4::getRotation(rotation) *
        mat4::getTranslation(position);
}

mat4 &Camera::getView()
{
    if(viewDirty)
    {
        calcView();
        viewDirty = false;
    }
    return view;
}

mat4 &Camera::getPerspective()
{
    return perspective;
}
