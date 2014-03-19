#ifndef _BRICKMENU_HPP
#define _BRICKMENU_HPP

#include <vector>
#include "brick.hpp"
#include "framework/draw/glTexture.hpp"
#include "framework/draw/drawDevice.hpp"


class BrickMenu
{
    GLTexture *background;
    std::vector< std::pair<GLTexture *, Brick::Type> > selections;

    public:
    void draw(DrawDevice *dev);
    BrickMenu();
};

#endif
