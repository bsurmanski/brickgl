#include "vector.hpp"

#include <math.h>
#include <string.h>

vec4::vec4()
{
    //memset(v, 0, sizeof(float[4]));
}

vec4::vec4(const vec4 &v)
{
    memcpy(this->v, v.v, sizeof(float[4])); 
}

vec4::vec4(float x, float y, float z, float w)
{
    this->x = x;
    this->y = y;
    this->z = z;
    this->w = w;
}

vec4& vec4::operator=(const vec4 &v)
{
    memcpy(this->v, v.v, sizeof(float[4]));
    return *this;
}

vec4 vec4::operator*(float f)
{
    vec4 ret;
    ret.x = x * f;
    ret.y = y * f;
    ret.z = z * f;
    ret.w = w * f;
    return ret;
}

vec4 vec4::operator+(vec4 &v)
{
    vec4 ret;
    ret.x = x + v.x;
    ret.y = y + v.y;
    ret.z = z + v.z;
    ret.w = w + v.w;
    return ret;
}

vec4 vec4::operator-(vec4 &v)
{
    vec4 ret;
    ret.x = x + v.x;
    ret.y = y + v.y;
    ret.z = z + v.z;
    ret.w = w + v.w;
    return ret;
}

void vec4::normalize()
{
    float lenInv = 1.0f / length();
    x = x * lenInv;
    y = y * lenInv;
    z = z * lenInv;
    w = w * lenInv;
}

// dot product
float vec4::operator*(vec4 &v)
{
    return dot(v);
}

float vec4::distance(vec4 &v)
{
    vec4 tmp = *this - v;
    return tmp.length();
}

float vec4::dot(vec4 &v)
{
    return x * v.x + y * v.y + z * v.z + w * v.w;
}

float vec4::length()
{
    sqrt(lengthSq());
}

float vec4::lengthSq()
{
    return dot(*this);
}

vec4 vec4::cross(vec4 &v)
{
    vec4 ret;
    ret.x = y * v.z - z * v.y;
    ret.y = z * v.x - x * v.z;
    ret.z = x * v.y - y * v.x;
    ret.w = 0.0f;
    return ret;
}

vec4 vec4::normalized()
{
    vec4 vv = *this;
    vv.normalize();
    return vv;
}

vec4 vec4::pow(int p)
{
    vec4 v = *this;
    v.x = powf(v.x, p);
    v.y = powf(v.y, p);
    v.z = powf(v.z, p);
    v.w = powf(v.w, p);
    return v;
}
