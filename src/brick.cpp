#include "brick.hpp"

bool Brick::collides(Brick &b2)
{
    return
        (right() > b2.left() && left() < b2.right()) && // x is intersecting
        (back() > b2.front() && front() < b2.back()) &&
        (bottom() < b2.top() && top() > b2.bottom());
}
