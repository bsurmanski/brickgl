#ifndef _QTWINDOW_HPP
#define _QTWINDOW_HPP

#include "window.hpp"

#include <string>

class QtWindowWidget;

class QtWindow : public Window {
    QtWindowWidget *widget;

    public:
    QtWindow(uint32_t w, uint32_t h, std::string name);
    virtual ~QtWindow();
    virtual void swapBuffers();
    virtual void clear();
};

#endif
