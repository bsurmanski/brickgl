#ifndef _BOX_HPP
#define _BOX_HPP

#include "vector.hpp"

class box
{
    vec4 position;
    vec4 dimension;

    public:

    box(vec4 p, vec4 d) : position(p), dimension(d) {}

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

    void print();

};

#endif
