#ifndef _IMAGE_HPP
#define _IMAGE_HPP

#include <stdint.h>
#include <stdlib.h>

class Image
{
    public:
    uint8_t *pixels;
    uint16_t w;
    uint16_t h;
    uint16_t format;

    enum Format
    {
        RGB8,
        RGBA8,
    };

    Image() : pixels(0) {}
    ~Image() 
    { 
        //free(pixels); 
        //TODO
        pixels = 0; 
    }

    uint32_t pitch();
    uint32_t length();
    uint32_t bitsPerPixel();
    uint32_t bytesPerPixel();
};

#endif
