#version 130
#extension GL_ARB_uniform_buffer_object : enable
#extension GL_ARB_explicit_attrib_location : require

//smooth in vec4 fcolor;
smooth in vec4 fposition;
smooth in vec4 fnormal;
smooth in vec2 fuv;

uniform sampler2D t_color;
uniform vec4 ambient;

layout(location = 0) out vec4 outColor;
layout(location = 1) out vec4 outNormal; 
layout(location = 2) out vec4 outPosition;
layout(location = 3) out vec4 outLight; // mess!

void main()
{
    outColor = texture(t_color, fuv);
    outNormal = fnormal;
    outPosition = fposition;
    outLight = ambient;
}
