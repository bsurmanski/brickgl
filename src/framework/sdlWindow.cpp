#include "sdlWindow.hpp"

SDLWindow::SDLWindow(uint32_t w, uint32_t h, std::string name) : Window(w, h, name)
{
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_NOPARACHUTE);
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    screen = SDL_SetVideoMode(w, h, 32, SDL_OPENGL);
    SDL_WM_SetCaption(name.c_str(), 0);
}

SDLWindow::~SDLWindow()
{
    SDL_Quit();
    this->~Window();
}

void SDLWindow::swapBuffers()
{
    SDL_Flip(screen);
}

void SDLWindow::clear()
{
    SDL_FillRect(screen, 0, 0);
    //TODO
}
