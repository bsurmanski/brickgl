#ifndef _DRAWDEVICE_HPP
#define _DRAWDEVICE_HPP

#include "../matrix.hpp"

class DrawDevice
{
    mat4 mMatrix;
    mat4 vMatrix;
    mat4 pMatrix;

    public:
    DrawDevice();
    virtual ~DrawDevice();
    
};

#endif
