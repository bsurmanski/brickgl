#include "matrix.hpp"

#include <stdio.h>
#include <math.h>
#include <float.h>
#include <string.h>
#include <assert.h>

void mat4::reset()
{
    v[0] = v[5] = v[10] = v[15] = 1.0f; 
    v[1] = v[2] = v[3] = v[4] = 
        v[6] = v[7] = v[8] = v[9] =
        v[11] = v[12] = v[13] = v[14] = 0.0f;
}

mat4::mat4(const mat4 &m)
{
    memcpy(this->v, m.v, sizeof(float[16]));
}

mat4& mat4::operator=(const mat4 &m)
{
    memcpy(this->v, m.v, sizeof(float[16]));
    return *this;
}

bool mat4::operator==(const mat4 &m)
{
    for(int i = 0; i < 16; i++)
    {
        if(fabs(v[i] - m.v[i]) > FLT_EPSILON) return false;
    }
    return true;
}


mat4 mat4::operator-(const mat4 &m)
{
    mat4 ret;
    for(int i = 0; i < 16; i++)
    {
        ret.v[i] = v[i] - m.v[i];
    }
    return ret;
}

mat4 mat4::operator+(const mat4 &m)
{
    mat4 ret;
    for(int i = 0; i < 16; i++)
    {
        ret.v[i] = v[i] + m.v[i];
    }
    return ret;
}

mat4 mat4::operator*(const mat4 &m)
{
    mat4 ret;
    ret.v[XX] = v[XX] * m.v[XX] + v[YX] * m.v[XY] +
                v[ZX] * m.v[XZ] + v[WX] * m.v[XW];
    ret.v[XY] = v[XY] * m.v[XX] + v[YY] * m.v[XY] +
                v[ZY] * m.v[XZ] + v[WY] * m.v[XW];
    ret.v[XZ] = v[XZ] * m.v[XX] + v[YZ] * m.v[XY] +
                v[ZZ] * m.v[XZ] + v[WZ] * m.v[XW];
    ret.v[XW] = v[XW] * m.v[XX] + v[YW] * m.v[XY] +
                v[ZW] * m.v[XZ] + v[WW] * m.v[XW];

    ret.v[YX] = v[XX] * m.v[YX] + v[YX] * m.v[YY] +
                v[ZX] * m.v[YZ] + v[WX] * m.v[YW];
    ret.v[YY] = v[XY] * m.v[YX] + v[YY] * m.v[YY] +
                v[ZY] * m.v[YZ] + v[WY] * m.v[YW];
    ret.v[YZ] = v[XZ] * m.v[YX] + v[YZ] * m.v[YY] +
                v[ZZ] * m.v[YZ] + v[WZ] * m.v[YW];
    ret.v[YW] = v[XW] * m.v[YX] + v[YW] * m.v[YY] +
                v[ZW] * m.v[YZ] + v[WW] * m.v[YW];

    ret.v[ZX] = v[XX] * m.v[ZX] + v[YX] * m.v[ZY] +
                v[ZX] * m.v[ZZ] + v[WX] * m.v[ZW];
    ret.v[ZY] = v[XY] * m.v[ZX] + v[YY] * m.v[ZY] +
                v[ZY] * m.v[ZZ] + v[WY] * m.v[ZW];
    ret.v[ZZ] = v[XZ] * m.v[ZX] + v[YZ] * m.v[ZY] +
                v[ZZ] * m.v[ZZ] + v[WZ] * m.v[ZW];
    ret.v[ZW] = v[XW] * m.v[ZX] + v[YW] * m.v[ZY] +
                v[ZW] * m.v[ZZ] + v[WW] * m.v[ZW];

    ret.v[WX] = v[XX] * m.v[WX] + v[YX] * m.v[WY] +
                v[ZX] * m.v[WZ] + v[WX] * m.v[WW];
    ret.v[WY] = v[XY] * m.v[WX] + v[YY] * m.v[WY] +
                v[ZY] * m.v[WZ] + v[WY] * m.v[WW];
    ret.v[WZ] = v[XZ] * m.v[WX] + v[YZ] * m.v[WY] +
                v[ZZ] * m.v[WZ] + v[WZ] * m.v[WW];
    ret.v[WW] = v[XW] * m.v[WX] + v[YW] * m.v[WY] +
                v[ZW] * m.v[WZ] + v[WW] * m.v[WW];
    return ret;
}

vec4 mat4::operator*(const vec4 &vv)
{
    vec4 ret;
    ret.x = v[XX] * vv.x + v[YX] * vv.y + v[ZX] * vv.z + v[WX] * vv.w;
    ret.y = v[XY] * vv.x + v[YY] * vv.y + v[ZY] * vv.z + v[WY] * vv.w;
    ret.z = v[XZ] * vv.x + v[YZ] * vv.y + v[ZZ] * vv.z + v[WZ] * vv.w;
    ret.w = v[XW] * vv.x + v[YW] * vv.y + v[ZW] * vv.z + v[WW] * vv.w;
    return ret;
}

mat4 mat4::getIdentity()
{
    mat4 ret;
    ret.reset();
    return ret;
}

mat4 mat4::getFrustum(float l, float r, float b, float t, float n, float f)
{
    mat4 m;
    m.v[XX] = 2.0f * n / (r - l);
    m.v[YX] = 0.0f;
    m.v[ZX] = (r + l) / (r - l);
    m.v[WX] = 0.0f;

    m.v[XY] = 0.0f;
    m.v[YY] = (2.0f * n) / (t - b);
    m.v[ZY] = (t + b) / (t - b);
    m.v[WY] = 0.0f;

    m.v[XZ] = 0.0f;
    m.v[YZ] = 0.0f;
    m.v[ZZ] = -(f + n) / (f - n);
    m.v[WZ] = -(2.0f  * f * n) / (f - n);

    m.v[XW] = 0.0f;
    m.v[YW] = 0.0f;
    m.v[ZW] = -1.0f;
    m.v[WW] = 0.0f;
    return m;
}

mat4 mat4::getOrtho(float l, float r, float b, float t, float n, float f)
{
    assert(false && "todo ortho");
}

mat4 mat4::getRotation(vec4 r)
{
    mat4 mx;
    mat4 my;
    mat4 mz;
    float cx, sx, cy, sy, cz, sz;
    sincosf(r.x, &sx, &cx);
    sincosf(r.y, &sy, &cy);
    sincosf(r.z, &sz, &cz);
    mx = my = mz = getIdentity();

    mx.v[YY] = cx;
    mx.v[ZY] = sx;
    mx.v[YZ] = -sx;
    mx.v[ZZ] = cx;

    my.v[XX] = cy;
    my.v[ZX] = -sy;
    my.v[XZ] = sy;
    my.v[ZZ] = cy;

    mz.v[XX] = cz;
    mz.v[YX] = sz;
    mz.v[XY] = -sz;
    mz.v[YY] = cz;

    return mx * my * mz;
}

mat4 mat4::getRotation(float angle, vec4 r)
{
    mat4 m;

    if(fabs(angle) < FLT_EPSILON) return getIdentity();
    float len = r.length();
    if(len < FLT_EPSILON) return getIdentity();
    r.normalize();

    float c, s, t;
    sincosf(angle, &s, &c);
    t = 1.0f - c;

    m.v[XX] = t * r.x * r.x + c;
    m.v[YX] = t * r.x * r.y - s * r.z;
    m.v[ZX] = t * r.x * r.z + s * r.y;

    m.v[WX] = m.v[WY] = m.v[WZ] = 0.0f;

    m.v[XY] = t * r.x * r.y + s * r.z;
    m.v[YY] = t * r.y * r.y + c; 
    m.v[ZY] = t * r.y * r.z - s * r.x;

    m.v[XZ] = t * r.x * r.z - s * r.y;
    m.v[YZ] = t * r.y * r.z + s * r.x;
    m.v[ZZ] = t * r.z * r.z + c;

    m.v[XW] = m.v[YW] = m.v[ZW] = 0.0f;
    m.v[WW] = 1.0f;

    return m;
}

mat4 mat4::getTranslation(vec4 v)
{
    mat4 m = getIdentity();
    m.v[WX] = v.x;
    m.v[WY] = v.y;
    m.v[WZ] = v.z;
    return m;
}


mat4 mat4::getScale(vec4 s)
{
    mat4 m = getIdentity();
    m.v[XX] = s.x;
    m.v[YY] = s.z;
    m.v[ZZ] = s.y;
    m.v[WW] = s.w;
    return m;
}

mat4 mat4::getScale(float f)
{
    return getScale(vec4(f, f, f, 1.0f));
}

void mat4::transpose()
{
    *this = transposed();
}

mat4 mat4::transposed()
{
    mat4 m;

    m.v[XX] = v[XX];
    m.v[XY] = v[YX];
    m.v[XZ] = v[ZX];
    m.v[XW] = v[WX];

    m.v[YX] = v[XY];
    m.v[YY] = v[YY];
    m.v[YZ] = v[ZY];
    m.v[YW] = v[WY];

    m.v[ZX] = v[XZ];
    m.v[ZY] = v[YZ];
    m.v[ZZ] = v[ZZ];
    m.v[ZW] = v[WZ];

    m.v[WX] = v[XW];
    m.v[WY] = v[YW];
    m.v[WZ] = v[ZW];
    m.v[WW] = v[WW];

    return m;
}

void mat4::translate(vec4 off)
{
    mat4 tmp = getTranslation(off); 
    *this = tmp * *this;
}

void mat4::scale(vec4 s)
{
    mat4 tmp = getScale(s); 
    *this = tmp * *this;
}

void mat4::scale(float f)
{
    mat4 tmp = getScale(vec4(f, f, f, 1.0f)); 
    *this = tmp * *this;
}

vec4 mat4::x()
{
    return vec4(v[XX], v[XY], v[XZ], v[XW]);
}

vec4 mat4::y()
{
    return vec4(v[YX], v[YY], v[YZ], v[YW]);
}

vec4 mat4::z()
{
    return vec4(v[ZX], v[ZY], v[ZZ], v[ZW]);
}

vec4 mat4::w()
{
    return vec4(v[WX], v[WY], v[WZ], v[WW]);
}

void mat4::print()
{
    printf("%f, %f, %f, %f\n", v[XX], v[YX], v[ZX], v[WX]);
    printf("%f, %f, %f, %f\n", v[XY], v[YY], v[ZY], v[WY]);
    printf("%f, %f, %f, %f\n", v[XZ], v[YZ], v[ZZ], v[WZ]);
    printf("%f, %f, %f, %f\n", v[XW], v[YW], v[ZW], v[WW]);
}
