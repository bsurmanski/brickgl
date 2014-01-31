#ifndef _INPUTDEVICE_HPP
#define _INPUTDEVICE_HPP

class InputDevice : public Device
{
    virtual bool keyDown(unsigned key) = 0;
    virtual void update(float dt) = 0;
};

#endif
