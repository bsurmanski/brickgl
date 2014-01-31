#version 130
#extension GL_ARB_uniform_buffer_object : enable
#extension GL_ARB_explicit_attrib_location : require

uniform mat4 mvpMatrix; // p_matrix * v_matrix * m_matrix

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv;

smooth out vec3 fNormal;
smooth out vec2 fUv;

void main()
{
    gl_Position = mvpMatrix * vec4(position, 1.0f);
    fNormal = (mvpMatrix * vec4(normal, 0.0f)).xyz;
    fNormal.z = -fNormal.z;
    fUv = uv;
}
