#ifndef _VECTOR_HPP
#define _VECTOR_HPP

class vec4
{
    public:
    union
    {
        float v[4];
        struct
        {
            float x;
            float y;
            float z;
            float w;
        };
        struct
        {
            float r;
            float g;
            float b;
            float a;
        };
    };

    float *ptr();
    vec4();
    vec4(const vec4 &v);
    vec4(float x, float y, float z, float w = 0.0f);
    vec4& operator=(const vec4 &v);
    vec4 operator*(float f);
    vec4 operator+(vec4 v);
    vec4 operator-(vec4 v);
    void normalize();
    float operator*(vec4 &v); // dot product
    float distance(vec4 &v);
    float distanceSq(vec4 &v);
    float dot(vec4 &v);
    float length();
    float lengthSq();
    vec4 cross(vec4 &v);
    vec4 normalized();
    vec4 pow(int p);

    void print();
};

#endif
