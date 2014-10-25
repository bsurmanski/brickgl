#ifndef _QTINPUTDEVICE_HPP
#define _QTINPUTDEVICE_HPP

#include "inputDevice.hpp"
#include <stdint.h>

#define NKEYS 128

class QtInputDevice : public InputDevice {
    unsigned mousex;
    unsigned mousey;

    // old mouse position
    unsigned omousex;
    unsigned omousey;

    uint8_t keystate[NKEYS];

    // old keystate
    uint8_t okeystate[NKEYS];

    public:
    QtInputDevice() : mousex(0), mousey(0) {
        for(int i = 0; i < NKEYS; i++) {
            okeystate[i] = keystate[i] = 0;
        }
    }

    void setKey(unsigned key, uint8_t val) { keystate[key] = val; }

    void setMouseX(unsigned x) { mousex = x; }
    void setMouseY(unsigned y) { mousey = y; }

    virtual bool isKeyDown(unsigned key);
    virtual bool isMouseDown(unsigned key);

    virtual bool keyPressed(unsigned key);
    virtual bool keyReleased(unsigned key);

    virtual unsigned mouseX();
    virtual unsigned mouseY();

    virtual int mouseDX();
    virtual int mouseDY();

    virtual void update(float dt);
};

#endif
