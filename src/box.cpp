#include "box.hpp"

#include<math.h>
#include<stdio.h>
#include<iostream>

float mtd(float p1, float d1, float p2, float d2)
{
    float ret;
    float v1 = p1 - (p2 + d2/2.0f); //dist of A's left to B's center
    float v2 = (p1 + d1) - (p2 + d2/2.0f); // dist of A's right point to B's center
    if(fabs(v1) > fabs(v2))
    {
        ret = (p2 + d2) - p1;
    } else
    {
        ret = p2 - (p1 + d1);
    }
    return ret;
}

bool overlaps(float p1, float d1, float p2, float d2)
{
    return ((p1 <= p2) && ((p1 + d1) > p2)) ||
           ((p2 <= p1) && ((p2 + d2) > p1));
}

bool between(float p1, float b1, float b2)
{
    return p1 >= b1 && p1 <= b2 ||
           p1 <= b1 && p1 >= b2;
}

/**
 * RAY
 */

bool ray::intersects(box b)
{
    vec4 d1 = position - b.position;
    vec4 d2 = position - (b.position + b.dimension);

    return 0; //TODO
}

/**
 * FACE
 */

vec4 face::getNormal()
{
    return (vertices[3] - vertices[0]).cross((vertices[1] - vertices[0])).normalized();
}

/**
 * BOX
 */

bool box::collides2(box &o)
{
    return overlaps(position.x, dimension.x, o.position.x, o.dimension.x) &&
           overlaps(position.y, dimension.y, o.position.y, o.dimension.y);
}

vec4 box::mtv2(box &o)
{
    vec4 r;
    r.x = mtd(position.x, dimension.x, o.position.x, o.dimension.x);
    r.y = mtd(position.y, dimension.y, o.position.y, o.dimension.y);
    r.z = 0;
    r.w = 0;
    return r;
}

bool box::collides3(box &o)
{
    bool ret = overlaps(position.x, dimension.x, o.position.x, o.dimension.x) &&
           overlaps(position.y, dimension.y, o.position.y, o.dimension.y) &&
           overlaps(position.z, dimension.z, o.position.z, o.dimension.z);

    printf("%d %d %d\n",
           overlaps(position.x, dimension.x, o.position.x, o.dimension.x),
           overlaps(position.y, dimension.y, o.position.y, o.dimension.y),
           overlaps(position.z, dimension.z, o.position.z, o.dimension.z));

    return ret;
}

vec4 box::mtv3(box &o)
{
    vec4 r;
    r.x = mtd(position.x, dimension.x, o.position.x, o.dimension.x);
    r.y = mtd(position.y, dimension.y, o.position.y, o.dimension.y);
    r.z = mtd(position.z, dimension.z, o.position.z, o.dimension.z);
    r.w = 0;
    return r;
}

vec4 box::offsetOf(vec4 v)
{
    vec4 ret = vec4(0,0,0,0);
    if(!between(v.x, position.x, position.x + dimension.x)){
        ret.x = (v.x < position.x) ? v.x - position.x : (position.x + dimension.x) - v.x;
    }

    if(!between(v.y, position.y, position.y + dimension.y)){
        ret.y = (v.y < position.y) ? v.y - position.y : (position.y + dimension.y) - v.y;
    }

    if(!between(v.z, position.z, position.z + dimension.z)){
        ret.z = (v.z < position.z) ? v.z - position.z : (position.z + dimension.z) - v.z;
    }

    return ret;
}

face box::getFace(int i)
{
    vec4 pos = position;
    vec4 dim;
    switch(i)
    {
        case 0:
            dim = vec4(dimension.x, dimension.y, 0, 0);
            break;

        case 1:
            pos.x += dimension.x;
            dim = vec4(0, dimension.y, dimension.z, 0);
            break;

        case 2:
            pos.x += dimension.x;
            pos.z += dimension.z;
            dim = vec4(-dimension.x, dimension.y, 0, 0);
            break;

        case 3:
            pos.z += dimension.z;
            dim = vec4(0, dimension.y, -dimension.z, 0);
            break;

        case 4:
            pos.x += dimension.x;
            dim = vec4(-dimension.x, 0, dimension.z, 0);
            break;

        case 5:
            pos.y += dimension.y;
            dim = vec4(dimension.x, 0, dimension.z, 0);
            break;

    }

    return face(pos, dim);
}

void box::print()
{
    std::cout << "box{p= ";
    position.print();
    std::cout << " d= ";
    dimension.print();
    std::cout << " }" << std::endl;
}
