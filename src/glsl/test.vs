#version 130
#extension GL_ARB_uniform_buffer_object : enable
#extension GL_ARB_explicit_attrib_location : require

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv;

smooth out vec4 color;

void main()
{
    gl_Position = vec4(position, 1.0f);
    color = vec4(uv, 1.0f, 1.0f);
}
