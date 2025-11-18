#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    FILE *fp = fopen("FirstPPM.ppm", "w");

    if (fp == NULL) {
        perror("Erreur d'ouverture du fichier");
        return EXIT_FAILURE;
    }

    fprintf(fp, "P3\n");

    fprintf(fp, "3 2\n");

    fprintf(fp, "255\n");

    fprintf(fp, "255 0 0 0 255 0 0 0 255\n");
    fprintf(fp, "255 255 0 255 255 255 0 0 0\n");

    fclose(fp); // Fermeture du fichier
    printf("Image PPM créée avec succès !\n");

    return EXIT_SUCCESS;
}