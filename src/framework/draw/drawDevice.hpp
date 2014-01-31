#ifndef _DRAWDEVICE_HPP
#define _DRAWDEVICE_HPP

#include "../../matrix.hpp"
#include "../device.hpp"
#include "drawProgram.hpp"

class DrawDevice : public Device
{
    public: //TODO: private
    mat4 mMatrix;
    mat4 vMatrix;
    mat4 pMatrix;

    public:
    DrawDevice();
    virtual ~DrawDevice();
    virtual void update(float dt); 
    virtual DrawProgram *createProgram() = 0;
};

#endif
