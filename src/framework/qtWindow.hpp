#ifndef _QTWINDOW_HPP
#define _QTWINDOW_HPP

#include "window.hpp"

#include <string>

#include <qt/QtGui/QtGui>
#include <qt/QtWidgets/QMainWindow>
#include <qt/QtWidgets/QMenuBar>

class GLFrame;

class QtWindow : public Window {
    QMainWindow *qwindow;

    QMenuBar *menubar;
    QMenu *fileMenu;
    QAction *saveAction;
    QAction *loadAction;
    QAction *quitAction;

    GLFrame *widget;

    public:
    QtWindow(Application *app, uint32_t w, uint32_t h, std::string name);
    virtual size_t frameHeight();
    virtual size_t frameWidth();
    virtual ~QtWindow();
    virtual void swapBuffers();
    virtual void clear();
};

#endif
