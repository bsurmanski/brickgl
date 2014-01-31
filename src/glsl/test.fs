#version 130
#extension GL_ARB_uniform_buffer_object : enable
#extension GL_ARB_explicit_attrib_location : require

smooth in vec4 color;

layout(location = 0) out vec4 outColor;

void main()
{
    outColor = color;
}
