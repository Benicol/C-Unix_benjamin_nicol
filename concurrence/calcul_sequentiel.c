#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <unistd.h>

#define TAILLE_DONNEES 10
#define MULTIPLICATEUR 1000

static const int DONNEES[TAILLE_DONNEES] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

int main(void) {
    printf("=== Calcul Séquentiel ===\n");

    struct timespec debut, fin;
    clock_gettime(CLOCK_MONOTONIC, &debut);

    int64_t somme_total = 0;

    // TODO: Ce code sera remplacé par une version parallèle
    for (int i = 0; i < TAILLE_DONNEES; i++) {
        int valeur = DONNEES[i];

        // Calcul intensif simulé par des opérations mathématiques
        int64_t resultat = 0;
        for (int j = 0; j < MULTIPLICATEUR; j++) {
            resultat += valeur * valeur + valeur;
            usleep(20);
        }

        somme_total += resultat;
        printf("Traitement de %d terminé : %ld\n", valeur, resultat);
    }

    clock_gettime(CLOCK_MONOTONIC, &fin);
    long duree = (fin.tv_sec - debut.tv_sec) * 1000 + 
                 (fin.tv_nsec - debut.tv_nsec) / 1000000;

    printf("Résultat total : %ld\n", somme_total);
    printf("Durée : %ld ms\n", duree);

    return 0;
}