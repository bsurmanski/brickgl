#ifndef _MATRIX_HPP
#define _MATRIX_HPP

#include "vector.hpp"

class mat4
{
    enum
    {
        XX = 0,
        XY,
        XZ,
        XW,
        YX,
        YY,
        YZ,
        YW,
        ZX,
        ZY,
        ZZ,
        ZW,
        WX,
        WY,
        WZ,
        WW
    };

    float v[16];
    public:

    static mat4 getIdentity();
    static mat4 getFrustum(float l, float r, float b, float t, float n, float f);
    static mat4 getOrtho(float l, float r, float b, float t, float n, float f);
    static mat4 getRotation(vec4 r);
    static mat4 getRotation(float angle, vec4 r);
    static mat4 getTranslation(vec4 v); // ignores v.w
    static mat4 getScale(vec4 s);

    mat4(){}

    mat4(const mat4 &m);

    bool operator==(const mat4 &m);
    mat4& operator=(const mat4 &m);
    mat4 operator-(const mat4 &m);
    mat4 operator+(const mat4 &m);
    mat4 operator*(const mat4 &m);
    vec4 operator*(const vec4 &m);

    void reset();
    void transpose();
    mat4 transposed();
    void normalize();
    mat4 normalized();
    void translate(vec4 off);
    float *ptr() { return v; }

    vec4 x();
    vec4 y();
    vec4 z();
    vec4 w();

    void print();
};

#endif
