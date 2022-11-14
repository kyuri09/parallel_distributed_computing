#ifndef PPM_H
#define PPM_H

typedef struct Pixel{
    unsigned char rgb[3];
} Pixel;

Pixel *flip(Pixel *tempPixel, int width, int height, int rank);
Pixel *grayscale(Pixel *tempPixel, int width, int height, int rank);
Pixel *smooth(Pixel *tempPixel, Pixel *up_low_row, int width, int height, int rank, int numprocs);

#endif
