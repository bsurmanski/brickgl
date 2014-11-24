#include "tgaFormat.hpp"

#include <errno.h>
#include <ctype.h>
#include <stdio.h>

enum ImageType {
    NO_IMAGE = 0,
    CMAP = 1,
    TRUECOLOR = 2,
    GRAYSCALE = 3,
    RLE_CMAP = 9,
    RLE_TRUECOLOR = 10,
    RLE_GRAYSCALE = 11
};

struct TGAImageSpec {
    uint16_t xorigin;
    uint16_t yorigin;
    uint16_t width;
    uint16_t height;
    uint8_t depth;
    uint8_t descriptor;
};

struct TGAHeader {
    uint8_t id;
    uint8_t cmap; //color map included?
    uint8_t itype; //image type
    uint8_t cmap_spec[5]; // color map spec
    TGAImageSpec ispec;
};

TGAHeader createHeader(int w, int h) {
    TGAHeader head;
    head.id = 0;
    head.cmap = 0;
    head.itype = TRUECOLOR;
    memset(&head.cmap_spec, 0, 5);

    TGAImageSpec ispec;
    ispec.xorigin = 0;
    ispec.yorigin = 0;
    ispec.width = w;
    ispec.height = h;
    ispec.depth = 32;
    ispec.descriptor = 0;
    head.ispec = ispec;

    return head;
}

void outputTGA(std::string filenm, Image &image) {
    TGAHeader head = createHeader(image.w, image.h);
    FILE *file = fopen(filenm.c_str(), "w+b");
    if(!file) {
        printf("SCREENSHOT ERROR %d\n", errno);
        exit(-1);
    }
    fwrite(&head, sizeof(head), 1, file);
    fwrite(image.pixels, image.w*image.h*4, 1, file);
    fclose(file);
}
