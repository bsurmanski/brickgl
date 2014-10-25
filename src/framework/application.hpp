#ifndef _APPLICATION_HPP
#define _APPLICATION_HPP

#include "draw/drawDevice.hpp"
#include "input/inputDevice.hpp"
#include "window.hpp"

class Application
{
    public:
    Window *window;
    DrawDevice *drawDevice;
    InputDevice *inputDevice;

    public:
    Application(){}
    Application(Window *window, DrawDevice *ddev, InputDevice *idev);
    virtual ~Application();

    DrawDevice *getDrawDevice() const { return drawDevice; }
    InputDevice *getInputDevice() const { return inputDevice; }

    virtual void init() {}
    virtual void input() {}
    virtual void draw() {}
    virtual void update(float dt) {
        if(drawDevice) drawDevice->update(dt);
        if(inputDevice) inputDevice->update(dt);
    }
    virtual void run() {}
};

#endif
