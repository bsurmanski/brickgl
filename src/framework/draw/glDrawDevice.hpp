#ifndef _GLDRAWDEVICE_HPP
#define _GLDRAWDEVICE_HPP

#define GL_GLEXT_PROTOTYPES

#include <GL/gl.h>
#include <GL/glext.h>

#include "drawDevice.hpp"

class GLDrawDevice : public DrawDevice
{

    public:
    GLDrawDevice();
    virtual ~GLDrawDevice();
};

#endif
