#include "brick.hpp"

bool Brick::collides(Brick &b2)
{
    return
        (right() > b2.left() && left() < b2.right()) && // x is intersecting
        (front() < b2.back() && back() > b2.front()) &&
        (bottom() < b2.top() && top() > b2.bottom());
}
