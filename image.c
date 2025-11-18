#include "image.h"
#include <stdlib.h>
#include <string.h>

Image* image_create(int width, int height) {
    Image *img = (Image *)malloc(sizeof(Image));
    img->width = width;
    img->height = height;
    img->pixels = (unsigned char *)malloc(3 * width * height * sizeof(unsigned char));
    return img;
}

void image_free(Image *img) {
    free(img->pixels);
    free(img);
}

void image_set_pixel(Image *img, int x, int y, unsigned char r, unsigned char g, unsigned char b) {
    if (img != NULL && x >= 0 && x < img->width && y >= 0 && y < img->height) {
        int index = (y * img->width + x) * 3;
        img->pixels[index + 0] = r;
        img->pixels[index + 1] = g;
        img->pixels[index + 2] = b;
    }
}

void image_save_txt(Image *img, const char *filename) {
    if (img == NULL) return;
    FILE *fp = fopen(filename, "wt");

    // Header
    fprintf(fp, "P3\n");
    fprintf(fp, "%d %d\n", img->width, img->height);
    fprintf(fp, "255\n");

    // Content
    for (int y = 0; y < img->height; y++)
    {
        for (int x = 0; x < img->width; x++)
        {
            int index = (y * img->width + x) * 3;
            fprintf(fp, "%d %d %d ",
                img->pixels[index + 0],
                img->pixels[index + 1],
                img->pixels[index + 2]);
        }
        fprintf(fp, "\n");
    }

    fclose(fp);
}