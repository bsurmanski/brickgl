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
#include <unistd.h>
#include <SDL/SDL.h>

#define GL_GLEXT_PROTOTYPES

#include <GL/gl.h>
#include <GL/glext.h>

#include "framework/input/qtInputDevice.hpp"
#include "framework/draw/glDrawDevice.hpp"
#include "framework/draw/glDrawProgram.hpp"
#include "framework/qtWindow.hpp"
#include "mainApplication.hpp"

#include <QtWidgets/QApplication>
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

    void quickScreenshot(std::string proj, std::string sstarget) {
        window = new QtWindow(this, 640, 480, "BrickSim", false);

        image_target = sstarget;
        willScreenshot = true;
        isRunning = false;
        load(proj);

        app.exec();
    }

    void quickInstructions(std::string proj, std::string qitarget) {
        window = new QtWindow(this, 640, 480, "BrickSim", false);

        image_target = qitarget;
        willGenInstructions = true;
        isRunning = false;
        load(proj);

        app.exec();
    }

    void update(float dt) {
        MainApplication::update(dt);

        if(!isRunning && !willScreenshot) {
            app.exit();
        }
    }
};

int main(int argc, char **argv)
{
    QtApplication app(argc, argv);

    std::string screenshotTarget = "";
    std::string instructionTarget = "";
    std::string quickload = "";

    int c;
    while(optind < argc) {
        c = getopt(argc, argv, "s:S:l:");
        switch(c) {
            case 's':
                screenshotTarget = optarg;
                break;
            case 'S':
                instructionTarget = optarg;
                break;
            case 'l':
                quickload = optarg;
                break;
            case '?':
                printf("unknown argument provided\n");
                exit(-1);
                break;
            case ':':
                printf("option requires argument\n");
                exit(-1);
                break;
        }
    }

    if(!screenshotTarget.empty()) {
        if(quickload.empty()) {
            printf("screenshot target requires a preloaded project: '-l\n");
            exit(-1);
        }
        app.quickScreenshot(quickload, screenshotTarget);
    } else if(!instructionTarget.empty()) {
        if(quickload.empty()) {
            printf("quick instruction target requires a preloaded project: '-l\n");
            exit(-1);
        }
        app.quickInstructions(quickload, instructionTarget);
    } else {
        app.run();
    }

    return 0;
}
