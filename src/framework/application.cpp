#include "application.hpp"

Application::Application(Window *win, DrawDevice *ddev, InputDevice *idev)
: window(win), drawDevice(ddev), inputDevice(idev)
{

}

Application::~Application()
{

}
