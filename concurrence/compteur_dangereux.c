#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define INCREMENT 11111111
#define THREAD_COUNT 10

static long long compteur_global = 0;  // Variable partagée MODIFIABLE et MODIFIÉ

typedef struct {
    int increments;
} thread_arg_t;

void reset_compteur(void) {
    compteur_global = 0;
}

int get_compteur(void) {
    return compteur_global;
}

void* incrementeur_thread(void* arg) {
    thread_arg_t* thread_arg = (thread_arg_t*) arg;
    for (int i = 0; i < thread_arg->increments; i++) {
        // Plusieurs thread incrémente en même temps le même nombre
        compteur_global++;  // RACE CONDITION ICI !
    }
    return NULL;
}

int executer_test(int nb_threads, int nb_increments) {
    reset_compteur();

    pthread_t* threads = malloc(nb_threads * sizeof(pthread_t));
    thread_arg_t* args = malloc(nb_threads * sizeof(thread_arg_t));

    // Création des threads
    for (int i = 0; i < nb_threads; i++)
        {
            args[i].increments = nb_increments;
            pthread_create(&threads[i], NULL, incrementeur_thread, &args[i]);
        }

    for (int i = 0; i < nb_threads; i++)
        pthread_join(threads[i], NULL);

    int resultat = get_compteur();

    free(threads);
    free(args);

    return resultat;
}

int main(void) {
    const int NB_TESTS = 50;
    const int NB_THREADS = 99;
    const int NB_INCREMENTS = 111111;
    const int ATTENDU = NB_THREADS * NB_INCREMENTS;

    printf("=== Test Race Condition ===\n");
    printf("Configuration : %d threads × %d incréments\n", NB_THREADS, NB_INCREMENTS);
    printf("Valeur attendue : %d\n\n", ATTENDU);

    int nb_corrects = 0;
    int min = ATTENDU;
    int max = 0;

    for (int test = 0; test < NB_TESTS; test++) {
        int resultat = executer_test(NB_THREADS, NB_INCREMENTS);

        if (resultat == ATTENDU) {
            nb_corrects++;
            printf("Test %2d : %d v \n", test + 1, resultat);
        } else {
            printf("Test %2d : %d x (différence: %d)\n", test + 1, resultat, ATTENDU - resultat);
        }

        if (resultat < min) min = resultat;
        if (resultat > max) max = resultat;
    }

    printf("\n=== Statistiques ===\n");
    printf("Tests corrects : %d/%d (%.1f%%)\n",
           nb_corrects, NB_TESTS, (100.0 * nb_corrects) / NB_TESTS);
    printf("Valeur min : %d (perte de %d)\n", min, ATTENDU - min);
    printf("Valeur max : %d\n", max);

    return 0;
}