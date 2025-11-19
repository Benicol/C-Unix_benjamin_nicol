#include "image.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

Image* image_create(int width, int height) {
    Image *img = (Image *)malloc(sizeof(Image));
    img->width = width;
    img->height = height;
    img->pixels = (unsigned char *)malloc(3 * width * height);
    return img;
}

Image* image_read_txt(const char *filename) {
    FILE *fp = 0;
    Image *img = 0;
    fp = fopen(filename, "r");

    if (fp == NULL)
        return NULL;

    char buffer[64];
    fgets(buffer, sizeof(buffer), fp);
    if (strncmp(buffer, "P3", 2) != 0) {
        fclose(fp);
        return NULL;
    }

    int width = 0, height = 0;
    fgets(buffer, sizeof(buffer), fp);
    sscanf(buffer, "%d %d", &width, &height);

    fgets(buffer, sizeof(buffer), fp);

    img = image_create(width, height);

    int big_buffer_size = 3 * width * height * 4; // *4 to be safe (la taille max d'un entier en ascii est 3 chiffres + espace)

    char big_buffer[big_buffer_size];

    while (fgets(big_buffer, big_buffer_size, fp)) {
        char *ptr = big_buffer;
        while (*ptr) {
            /* Skip whitespace */
            while (isspace((unsigned char)*ptr)) ptr++;

            if (*ptr == '\0' || *ptr == '#') break; /* end of line or comment */

            for (int i = 0; i < 3; i++) {
                int value;
                int consumed = 0;
                if (sscanf(ptr, "%d%n", &value, &consumed) != 1) { /* %n me permet de savoir combien de caractères ont été consommés et donc de décallé le pointeur correctement ensuite*/
                    ptr += strlen(ptr);
                    break;
                }
                static int pixelIndex = 0;
                img->pixels[pixelIndex++] = (unsigned char)value;
                ptr += consumed;
            }
        }
    }

    fclose(fp);
    return img;
}


void image_free(Image *img) {
    free(img->pixels);
    free(img);
}

void image_set_pixel(Image *img, int x, int y, unsigned char r, unsigned char g, unsigned char b) {
    if (img != NULL && x >= 0 && x < img->width && y >= 0 && y < img->height) {
        int index = (y * img->width + x) * 3;
        img->pixels[index] = r;
        img->pixels[index + 1] = g;
        img->pixels[index + 2] = b;
    }
}

// Implementations for channel getters
unsigned char image_get_red(Image *img, int x, int y) {
    if (img == NULL) return 0;
    if (x < 0 || x >= img->width || y < 0 || y >= img->height) return 0;
    int index = (y * img->width + x) * 3;
    return img->pixels[index];
}

unsigned char image_get_green(Image *img, int x, int y) {
    if (img == NULL) return 0;
    if (x < 0 || x >= img->width || y < 0 || y >= img->height) return 0;
    int index = (y * img->width + x) * 3;
    return img->pixels[index + 1];
}

unsigned char image_get_blue(Image *img, int x, int y) {
    if (img == NULL) return 0;
    if (x < 0 || x >= img->width || y < 0 || y >= img->height) return 0;
    int index = (y * img->width + x) * 3;
    return img->pixels[index + 2];
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
                img->pixels[index],
                img->pixels[index + 1],
                img->pixels[index + 2]);
        }
        fprintf(fp, "\n");
    }

    fclose(fp);
}

void image_save_bin(Image *img, const char *filename) {
    if (img == NULL) return;
    FILE *fp = fopen(filename, "wb");

    // Header
    fprintf(fp, "P6\n");
    fprintf(fp, "%d %d\n", img->width, img->height);
    fprintf(fp, "255\n");

    // Content (binary)
    int total = 3 * img->width * img->height;
    fwrite(img->pixels, 1, total, fp);

    fclose(fp);
}

Image* image_read_bin(const char *filename) {
    FILE *fp = 0;
    Image *img = 0;
    fp = fopen(filename, "rb");

    if (fp == NULL)
        return NULL;

    char buffer[64];
    if (!fgets(buffer, sizeof(buffer), fp)) {
        fclose(fp);
        return NULL;
    }
    if (strncmp(buffer, "P6", 2) != 0) {
        fclose(fp);
        return NULL;
    }

    int width = 0, height = 0;
    if (!fgets(buffer, sizeof(buffer), fp)) {
        fclose(fp);
        return NULL;
    }
    if (sscanf(buffer, "%d %d", &width, &height) != 2) {
        fclose(fp);
        return NULL;
    }

    if (!fgets(buffer, sizeof(buffer), fp)) {
        fclose(fp);
        return NULL;
    }

    int maxval = atoi(buffer); // atoi transforms une chaîne de caractères en entier
    if (width <= 0 || height <= 0 || maxval <= 0 || maxval > 255) {
        fclose(fp);
        return NULL;
    }

    img = image_create(width, height);
    if (!img) { fclose(fp); return NULL; }

    int total = 3 * img->width * img->height;
    int read = fread(img->pixels, 1, total, fp);
    if (read != total) {
        image_free(img);
        fclose(fp);
        return NULL;
    }

    fclose(fp);
    return img;
}