#ifndef _BOX_HPP
#define _BOX_HPP

#include "vector.hpp"
#include <stdio.h>
#include <assert.h>
#include <math.h>

class box;

class ray
{
    vec4 position;
    vec4 direction;

    public:

    ray(vec4 p, vec4 d) : position(p), direction(d) {
    }

    void setPosition(vec4 p) { position = p; }
    vec4 getPosition() { return position; }

    void setDirection(vec4 d) { direction = d; }
    vec4 getDirection() { return direction; }

    bool intersects(box b);

};

class face
{
    vec4 vertices[4]; // in CCW order

    public:

    face(vec4 a, vec4 b, vec4 c, vec4 d){
        vertices[0] = a;
        vertices[0] = b;
        vertices[0] = c;
        vertices[0] = d;
    }
    face(vec4 corner, vec4 dim)
    {
        assert(dim.x >= 0);
        assert(dim.y >= 0); // XXX must be positive for now
        assert(dim.z >= 0);
        vertices[0] = corner;
        vertices[1] = corner + vec4(dim.x, 0, 0, 0);
        vertices[2] = corner + vec4(dim.x, dim.y, 0, 0);
        vertices[3] = corner + vec4(0,     dim.y, 0, 0);
    }

    vec4 getNormal();
};

class box
{
    vec4 position;
    vec4 dimension;

    public:

    box(vec4 p, vec4 d) : position(p), dimension(d) {
        if(dimension.y < 0)
        {
            printf("\n:");
            this->print();
        }
        if(dimension.x < 0)
        {
            position.x = position.x + dimension.x;
            dimension.x = fabs(dimension.x);
        }

        if(dimension.y < 0)
        {
            position.y = position.y + dimension.y;
            dimension.y = fabs(dimension.y);
        }

        if(dimension.z < 0)
        {
            position.z = position.z + dimension.z;
            dimension.z = fabs(dimension.z);
        }

        if(dimension.w < 0)
        {
            position.w = position.w + dimension.w;
            dimension.w = fabs(dimension.w);
        }
    }

    //shrinks the box by 'f' in all dimensions to allow for
    // slight error
    void trim(float f) {
        position.x += f;
        position.y += f;
        position.z += f;
        dimension.x -= f;
        dimension.y -= f;
        dimension.z -= f;
    }

    void setPosition(vec4 p) { position = p; }
    vec4 getPosition() { return position; }

    void setCenter(vec4 p) { position = p - (dimension / 2.0f); }
    vec4 getCenter() { return position + (dimension / 2.0f); }

    void setDimension(vec4 d) { dimension = d; }
    vec4 getDimension() { return dimension; }

    void move(vec4 v) { position = position + v; }
    //void scale(vec4 sc) { dimension = dimension * sc; }

    bool collides2(box &o);
    vec4 mtv2(box &o);

    bool collides3(box &o);
    vec4 mtv3(box &o);

    vec4 offsetOf(vec4 v);

    face getFace(int i);

    void print();

/*
#include "framework/draw/glDrawDevice.hpp"
#include "framework/draw/mesh.hpp"
    void debugDraw(DrawDevice *dev);
*/

    friend ray;

};

#endif
