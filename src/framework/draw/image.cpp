#include "image.hpp"

uint32_t Image::pitch()
{
    return bytesPerPixel() * h;
}

uint32_t Image::length()
{
    return bytesPerPixel() * w * h;
}

uint32_t Image::bitsPerPixel()
{
    return bytesPerPixel() * 8;
}

uint32_t Image::bytesPerPixel()
{
    switch(format)
    {
        case RGB8:
            return 3;
        case RGBA8:
            return 4;
        default:
            return 0;
    }
}
