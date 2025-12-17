#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>

#define TAILLE_DONNEES 10
#define MULTIPLICATEUR 1000

static const int DONNEES[TAILLE_DONNEES] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

typedef struct thread_info_data
{
    int valeur;
    int multiplicateur;
    int resultat;
} thread_info_data;

void* calcul_thread(void* arg)
{
    thread_info_data* data = (thread_info_data*)arg;
    int resultat = 0;
    int multiplicateur = data->multiplicateur;
    int valeur = data->valeur;
    for (int j = 0; j < multiplicateur; j++)
    {
        resultat += valeur * valeur + valeur;
        usleep(20);
    }
    data -> resultat = resultat;
    return data;
}


int main(void) {
    printf("=== Calcul Parallèle ===\n");

    struct timespec debut, fin;
    clock_gettime(CLOCK_MONOTONIC, &debut);

    int64_t somme_total = 0;

    // TODO: Créer un tableau de pthread_t
    pthread_t threads[TAILLE_DONNEES];
    // TODO: Créer un tableau de structures d'arguments
    thread_info_data thread_data[TAILLE_DONNEES];
    // TODO: Créer et démarrer tous les threads
    for (int i = 0; i < TAILLE_DONNEES; i++)
    {
        thread_data[i].valeur = DONNEES[i];
        thread_data[i].multiplicateur = MULTIPLICATEUR;
        pthread_create(&threads[i], NULL, calcul_thread, &thread_data[i]);
    }
    // TODO: Attendre tous les threads
    // TODO: Agréger les résultats
    for (int i = 0; i < TAILLE_DONNEES; i++)
    {
        pthread_join(threads[i], NULL);
        somme_total += thread_data[i].resultat;
    }

    clock_gettime(CLOCK_MONOTONIC, &fin);
    long duree = (fin.tv_sec - debut.tv_sec) * 1000 + 
                 (fin.tv_nsec - debut.tv_nsec) / 1000000;

    printf("Résultat total : %ld\n", somme_total);
    printf("Durée : %ld ms\n", duree);

    return 0;
}