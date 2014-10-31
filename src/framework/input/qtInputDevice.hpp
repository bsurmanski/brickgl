#ifndef _QTINPUTDEVICE_HPP
#define _QTINPUTDEVICE_HPP

#include "inputDevice.hpp"
#include <stdint.h>

#define NKEYS 128

class QtInputDevice : public InputDevice {
    unsigned mousex;
    unsigned mousey;
    int wheelx;
    int wheely;

    // old mouse position
    unsigned omousex;
    unsigned omousey;
    int owheelx;
    int owheely;

    uint8_t keystate[NKEYS];

    // old keystate
    uint8_t okeystate[NKEYS];

    public:
    QtInputDevice() : mousex(0), mousey(0), wheelx(0), wheely(0) {
        for(int i = 0; i < NKEYS; i++) {
            okeystate[i] = keystate[i] = 0;
        }
    }

    void setKey(unsigned key, uint8_t val) { keystate[key] = val; }

    void setMouseX(unsigned x) { mousex = x; }
    void setMouseY(unsigned y) { mousey = y; }
    void setWheelDX(int x) { wheelx = x; }
    void setWheelDY(int y) { wheely = y; }

    virtual bool isKeyDown(unsigned key);
    virtual bool isMouseDown(unsigned key);

    virtual bool keyPressed(unsigned key);
    virtual bool keyReleased(unsigned key);

    virtual unsigned mouseX();
    virtual unsigned mouseY();

    virtual int mouseDX();
    virtual int mouseDY();

    virtual int wheelDX();
    virtual int wheelDY();

    virtual void update(float dt);
};

#endif
