#include "pngFormat.hpp"

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

// XXX cheat by using SDL IMG_Load. maybe reimplement later, for portability
// probably just use TGA later
Image pngLoad(std::string filenm)
{
    SDL_Surface *surf = IMG_Load(filenm.c_str());
    if(!surf) return Image();

    Image image;
    image.format = 0;
    if(surf->format->BytesPerPixel == 4) image.format = Image::RGBA8;
    else if(surf->format->BytesPerPixel == 3) image.format = Image::RGB8;
    image.w = surf->w;
    image.h = surf->h;
    image.pixels = (uint8_t*) malloc(image.length() * image.bytesPerPixel());

    memcpy(image.pixels, surf->pixels, surf->pitch * surf->w);
    SDL_FreeSurface(surf);
    return image;
}
