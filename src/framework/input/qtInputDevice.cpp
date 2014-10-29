#include "qtInputDevice.hpp"

#include <string.h>

bool QtInputDevice::isKeyDown(unsigned key) {
    return keystate[key];
}

bool QtInputDevice::isMouseDown(unsigned key) {
    return keystate[key];
}

bool QtInputDevice::keyPressed(unsigned key) {
    return keystate[key] && !okeystate[key];
}

bool QtInputDevice::keyReleased(unsigned key) {
    return !keystate[key] && okeystate[key];
}

unsigned QtInputDevice::mouseX() {
    return mousex;
}

unsigned QtInputDevice::mouseY() {
    return mousey;
}

int QtInputDevice::mouseDX() {
    return mousex - omousex;
}

int QtInputDevice::mouseDY() {
    return mousey - omousey;
}


void QtInputDevice::update(float dt) {
    omousex = mousex;
    omousey = mousey;
    memcpy(okeystate, keystate, sizeof(uint8_t) * NKEYS);
}