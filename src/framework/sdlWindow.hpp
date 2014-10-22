#ifndef _SDLWINDOW_HPP
#define _SDLWINDOW_HPP

#include <SDL/SDL.h>
#include "window.hpp"

class SDLWindow : public Window
{
    SDL_Surface *screen;

    public:
    SDLWindow(Application *app, uint32_t w, uint32_t h, std::string name);
    virtual ~SDLWindow();
    virtual void swapBuffers();
    virtual void clear();
};

#endif
