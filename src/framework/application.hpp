#ifndef _APPLICATION_HPP
#define _APPLICATION_HPP

#include "draw/drawDevice.hpp"
#include "window.hpp"

class Application
{
    public:
    Window *window;
    DrawDevice *drawDevice;

    public:
    Application(){}
    Application(Window *window, DrawDevice *ddev);
    virtual ~Application();
};

#endif
