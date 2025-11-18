#include "image.h"
#include <stdio.h>

#define WIDTH 200
#define HEIGHT 100

int main(int argc, char *argv[]) {
    Image *img = image_create(WIDTH, HEIGHT);

    if (img == NULL) {
        return 1;
    }

    float width_per_pixel = 255 / (float) WIDTH;
    for (int x = 0; x < WIDTH; x++)
    {
        int blue = (int)(width_per_pixel * (float)x);
        for (int y = 0; y < HEIGHT; y++)
        {
            image_set_pixel(img, x, y, 0, 0, blue);
        }
    }


    image_save_txt(img, "gradient.ppm");

    image_free(img);
    return 0;
}
