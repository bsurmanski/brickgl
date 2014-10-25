#ifndef _INPUTDEVICE_HPP
#define _INPUTDEVICE_HPP

#include "../device.hpp"

enum {
    KEY_UNKNOWN,
    KEY_LEFT,
    KEY_UP,
    KEY_RIGHT,
    KEY_DOWN,
    KEY_SHIFT,
    KEY_ESC,
    KEY_SPACE,
    KEY_ENTER,
    KEY_TAB,

    MOUSE_LEFT,
    MOUSE_MIDDLE,
    MOUSE_RIGHT,

    KEY_0 = '0',
    KEY_1 = '1',
    KEY_2 = '2',
    KEY_3 = '3',
    KEY_4 = '4',
    KEY_5 = '5',
    KEY_6 = '6',
    KEY_7 = '7',
    KEY_8 = '8',
    KEY_9 = '9',

    KEY_A = 'a',
    KEY_B = 'b',
    KEY_C = 'c',
    KEY_D = 'd',
    KEY_E = 'e',
    KEY_F = 'f',
    KEY_G = 'g',
    KEY_H = 'h',
    KEY_I = 'i',
    KEY_J = 'j',
    KEY_L = 'l',
    KEY_M = 'm',
    KEY_N = 'n',
    KEY_O = 'o',
    KEY_P = 'p',
    KEY_Q = 'q',
    KEY_R = 'r',
    KEY_S = 's',
    KEY_T = 't',
    KEY_U = 'u',
    KEY_V = 'v',
    KEY_W = 'w',
    KEY_X = 'x',
    KEY_Y = 'y',
    KEY_Z = 'z',
};

class InputDevice : public Device
{
    public:
    InputDevice(){}
    virtual bool isKeyDown(unsigned key) = 0;
    virtual bool isMouseDown(unsigned key) = 0;

    virtual bool keyPressed(unsigned key) = 0;
    virtual bool keyReleased(unsigned key) = 0;

    virtual unsigned mouseX() = 0;
    virtual unsigned mouseY() = 0;
    virtual int mouseDX() = 0;
    virtual int mouseDY() = 0;
    virtual void update(float dt) {}
};

#endif
