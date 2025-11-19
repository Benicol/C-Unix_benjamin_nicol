#include "image.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    // Vérification des arguments
    if (argc < 4) {
        fprintf(stderr, "Usage: %s <input_file> <output_file> <filter_type>\n", argv[0]);
        fprintf(stderr, "Vous devez fournir des noms de fichiers d'entrée et de sortie et un type de filtre. Abandon\n");
        return -1;
    }

    const char *inputFilename = argv[1];
    const char *outputFilename = argv[2];
    const char *filterType = argv[3];

    Image *source = image_read_bin(inputFilename);
    if (source == NULL) {
        fprintf(stderr, "Erreur lecture fichier d'entrée\n");
        return -1;
    }

    int h = source->height;
    int w = source->width;
    Image *destination = image_create(w, h);

    if (destination == NULL) {
        image_free(source);
        return -1;
    }

    // Application du filtre pixel par pixel
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            unsigned char rouge = image_get_red(source, x, y);
            unsigned char vert = image_get_green(source, x, y);
            unsigned char bleu = image_get_blue(source, x, y);

            if (strcmp(filterType, "copy") == 0) {
                image_set_pixel(destination, x, y, rouge, vert, bleu);
            }
            else if (strcmp(filterType, "dark") == 0) {
                image_set_pixel(destination, x, y, rouge / 2, vert / 2, bleu / 2);
            }
            else if (strcmp(filterType, "bright") == 0) {
                unsigned char r = (rouge > 128) ? 255 : rouge * 2;
                unsigned char g = (vert > 128) ? 255 : vert * 2;
                unsigned char b = (bleu > 128) ? 255 : bleu * 2;
                image_set_pixel(destination, x, y, r, g, b);
            }
            else if (strcmp(filterType, "grayscale") == 0) {
                unsigned char gray = (unsigned char)(0.299 * rouge + 0.587 * vert + 0.114 * bleu);
                image_set_pixel(destination, x, y, gray, gray, gray);
            }
            else {
                fprintf(stderr, "Type de filtre inconnu: %s\n", filterType);
                image_free(source);
                image_free(destination);
                return -1;
            }
        }
    }

    // Sauvegarde de l'image filtrée
    image_save_bin(destination, outputFilename);
    printf("Image filtrée sauvegardée dans %s\n", outputFilename);

    image_free(source);
    image_free(destination);

    return 0;
}