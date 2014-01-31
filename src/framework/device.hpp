#ifndef _DEVICE_HPP
#define _DEVICE_HPP

class Device
{
    public:
    virtual ~Device(){}
    virtual void update(float dt) = 0;
};

#endif
