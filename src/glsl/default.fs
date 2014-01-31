#version 130
#extension GL_ARB_uniform_buffer_object : enable
#extension GL_ARB_explicit_attrib_location : require

uniform sampler2D tex0;

smooth in vec4 fPosition;
smooth in vec3 fNormal;
smooth in vec2 fUv;

layout(location = 0) out vec4 outColor;
layout(location = 1) out vec4 outNormal;

void main()
{
    outColor = texture(tex0, fUv);
    outNormal = vec4(normalize(fNormal), 1.0f);
}
