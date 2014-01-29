#include "window.hpp"

Window::Window(uint32_t w, uint32_t h, std::string name)
{
    this->width = w;
    this->height = h;
    this->name = name;
}

Window::~Window()
{

}

uint32_t Window::getWidth()
{
    return width;
}

uint32_t Window::getHeight()
{
    return height;
}

std::string Window::getName()
{
    return name;
}

void Window::setWidth(uint32_t w)
{
    width = w; 
}

void Window::setHeight(uint32_t h)
{
    height = h;
}

void Window::setName(std::string nm)
{
    name = nm;
}
