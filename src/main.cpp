/**
 * main.c
 * brickgl
 * January 21, 2014
 * Brandon Surmanski
 */

#include <stddef.h>
#include <stdbool.h>
#include <limits.h>
#include <math.h>
#include <SDL/SDL.h>

#define GL_GLEXT_PROTOTYPES

#include <GL/gl.h>
#include <GL/glext.h>

#include "framework/input/qtInputDevice.hpp"
#include "framework/draw/glDrawDevice.hpp"
#include "framework/draw/glDrawProgram.hpp"
#include "framework/qtWindow.hpp"
#include "mainApplication.hpp"

#include <qt/QtWidgets/QApplication>
#include <vector>

vec4 target;


class QtApplication : public MainApplication {
    QApplication app;

    public:
    QtApplication(int argc, char **argv) : MainApplication(argc, argv), app(argc, argv) {
    }

    // called by GLFrame, hanging out with QtWindow
    void init() {
        inputDevice = new QtInputDevice;
        MainApplication::init();
    }

    void run() {
        window = new QtWindow(this, 640, 480, "BrickSim");
        app.exec();
    }
};

int main(int argc, char **argv)
{
    QtApplication app(argc, argv);
    app.run();
    return 0;
}
