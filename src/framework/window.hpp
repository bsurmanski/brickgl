#ifndef _WINDOW_HPP
#define _WINDOW_HPP

#include <stdint.h>
#include <string>

#include "draw/drawDevice.hpp"

class Application;
class Window
{
    uint32_t width;
    uint32_t height;
    std::string name;

    DrawDevice *draw;

    public:
    Window(Application *app, uint32_t w, uint32_t h, std::string name);
    virtual ~Window();
    virtual void swapBuffers() = 0;
    virtual void clear() = 0;

    uint32_t getWidth();
    uint32_t getHeight();
    std::string getName();

    virtual void setWidth(uint32_t w);
    virtual void setHeight(uint32_t h);
    virtual void setName(std::string n);

    virtual size_t frameHeight() = 0;
    virtual size_t frameWidth() = 0;
};

#endif
