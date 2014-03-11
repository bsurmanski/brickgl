#ifndef _CAMERA_HPP
#define _CAMERA_HPP

#include "../../vector.hpp"
#include "../../matrix.hpp"

class Camera
{
    vec4 position;
    vec4 offset;
    vec4 rotation;

    bool viewDirty;
    mat4 view;
    mat4 perspective;

    void calcView();

    public:
    Camera();

    void setRotation(vec4 v) { viewDirty = true; rotation = v; }
    void addRotation(vec4 v) { viewDirty = true; rotation = rotation + v; }
    vec4 getRotation() { return rotation; }

    void setOffset(vec4 v) { viewDirty = true; offset = v; }
    void addOffset(vec4 v) { viewDirty = true; offset = offset + v; }
    vec4 getOffset() { return offset; }

    void setPosition(vec4 v) { viewDirty = true; position = v; }
    void addPosition(vec4 v) { viewDirty = true; position = position + v; }
    vec4 getPosition() { return position; }

    mat4 &getView();
    mat4 &getPerspective();
};

#endif
