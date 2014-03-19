#include "brickMenu.hpp"

#include "framework/draw/pngFormat.hpp"

BrickMenu::BrickMenu()
{
    background = new GLTexture(pngLoad("res/sidebar_bg.png"));
}

void BrickMenu::draw(DrawDevice *dev)
{
    //((GLDrawDevice*) dev)->drawFlat(background, vec4(0,0,1,1));
}
