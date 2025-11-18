#ifndef IMAGE_H
#define IMAGE_H

#include <stdio.h>

typedef struct {
    int width;
    int height;
    // pixels[y][x][0=R,1=G,2=B]
    unsigned char *pixels;
} Image;

Image* image_create(int width, int height);
void image_free(Image *img);
void image_set_pixel(Image *img, int x, int y, unsigned char r, unsigned char g, unsigned char b);
unsigned char image_get_red(Image *img, int x, int y);
unsigned char image_get_green(Image *img, int x, int y);
unsigned char image_get_blue(Image *img, int x, int y);
void image_save_txt(Image *img, const char *filename);
void image_save_bin(Image *img, const char *filename);
Image* image_read_txt(const char *filename);
Image* image_read_bin(const char *filename);

#endif
