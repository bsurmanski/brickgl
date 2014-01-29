#ifndef _GLDRAWDEVICE_HPP
#define _GLDRAWDEVICE_HPP

#include <GL/gl.h>
#include <GL/glext.h>

#include "drawDevice.hpp"

class GLDrawDevice : public DrawDevice
{
    GLDrawDevice();
    virtual ~GLDrawDevice();
};

#endif
