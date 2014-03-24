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

void box::print()
{
    std::cout << "box{p= ";
    position.print();
    std::cout << " d= ";
    dimension.print();
    std::cout << " }" << std::endl;
}
