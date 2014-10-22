#ifndef _QTWINDOW_HPP
#define _QTWINDOW_HPP

#include "window.hpp"

#include <string>

#include <qt/QtGui/QtGui>

class GLFrame;

class QtWindow : public Window {
    GLFrame *widget;

    public:
    QtWindow(Application *app, uint32_t w, uint32_t h, std::string name);
    virtual ~QtWindow();
    virtual void swapBuffers();
    virtual void clear();
};

#endif
