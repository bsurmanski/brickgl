#version 130
#extension GL_ARB_uniform_buffer_object : enable
#extension GL_ARB_explicit_attrib_location : require

uniform mat4 mvpMatrix;

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv;

smooth out vec4 color;
smooth out vec4 fnormal;
smooth out vec2 fuv;

void main()
{
    gl_Position = mvpMatrix * vec4(position, 1.0f);
    fnormal = mvpMatrix * vec4(normal, 0.0f);
    fuv = uv;
    color = vec4(position, 1.0f);
}
