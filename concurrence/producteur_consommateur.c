#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define TAILLE_BUFFER 5

// Buffer circulaire
static int buffer[TAILLE_BUFFER];
static int index_prod = 0;
static int index_cons = 0;

static sem_t places_libres;
static sem_t elements_disponibles;
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

static void init_semaphores(void) {
    sem_init(&places_libres, 0, TAILLE_BUFFER);
    sem_init(&elements_disponibles, 0, 0);
    pthread_mutex_init(&mutex, NULL);
}

void ajouter_buffer(int produit) {
    pthread_mutex_lock(&mutex);
    buffer[index_prod] = produit;
    index_prod = (index_prod + 1) % TAILLE_BUFFER;
    pthread_mutex_unlock(&mutex);
}

int retirer_buffer(void) {
    pthread_mutex_lock(&mutex);
    int produit = buffer[index_cons];
    index_cons = (index_cons + 1) % TAILLE_BUFFER;
    pthread_mutex_unlock(&mutex);
    return produit;
}

typedef struct
{
    int id;
    int nb;
} thread_info_data;
void* thread_producteur(void* arg) {
    thread_info_data* info = (thread_info_data*)arg;
    int id = info->id;
    int nb_producteurs = info->nb;

    for (int i = 0; i < nb_producteurs; i++) {
        int produit = id * 100 + i;

        sem_wait(&places_libres);
        ajouter_buffer(produit);

        printf("[Producteur %d] Ajouté : %d (index=%d)\n",
               id, produit, (index_prod - 1 + TAILLE_BUFFER) % TAILLE_BUFFER);

        sem_post(&elements_disponibles);
        usleep(50);
    }

    printf("[Producteur %d] terminé (10 produits)\n", id);
    return NULL;
}


void* thread_consommateur(void* arg) {
    thread_info_data* info = (thread_info_data*)arg;
    int id = info->id;
    int nb_consommateurs = info->nb;

    for (int i = 0; i < nb_consommateurs; i++) {
        sem_wait(&elements_disponibles);
        int produit = retirer_buffer();

        printf("[Consommateur %d] Retiré : %d (index=%d)\n",
               id, produit, (index_cons - 1 + TAILLE_BUFFER) % TAILLE_BUFFER);

        sem_post(&places_libres);

        usleep(50);
    }

    printf("[Consommateur %d] terminé (7 produits)\n", id);
    return NULL;
}


void do_test(int nb_producteurs, int nb_consommateurs, int produits_par_producteur, int produits_par_consommateur)
{
    pthread_t producteurs[nb_producteurs];
    pthread_t consommateurs[nb_consommateurs];
    thread_info_data ids_prod[nb_producteurs];
    thread_info_data ids_cons[nb_consommateurs];

    // Initialiser les structure d'info
    for (int i = 0; i < nb_producteurs; i++)
    {
        ids_prod[i].id = i;
        ids_prod[i].nb = produits_par_producteur;
    }
    for (int i = 0; i < nb_consommateurs; i++)
    {
        ids_cons[i].id = i;
        ids_cons[i].nb = produits_par_consommateur;
    }

    // Demarrer les threads
    for (int i = 0; i < nb_producteurs; i++)
        pthread_create(&producteurs[i], NULL, thread_producteur, &ids_prod[i]);
    for (int i = 0; i < nb_consommateurs; i++)
        pthread_create(&consommateurs[i], NULL, thread_consommateur, &ids_cons[i]);

    // Attendre 5 Secondes
    sleep(5);

    // Annuler tous les threads
    for (int i = 0; i < nb_producteurs; i++)
        pthread_cancel(producteurs[i]);
    for (int i = 0; i < nb_consommateurs; i++)
        pthread_cancel(consommateurs[i]);

    // Rattacher (join) tous les threads et informer de leur annulation
    for (int i = 0; i < nb_producteurs; i++)
    {
        void *ret;
        pthread_join(producteurs[i], &ret);
        if (ret == PTHREAD_CANCELED)
            printf("[Producteur %d] annulé\n", ids_prod[i].id);
    }
    for (int i = 0; i < nb_consommateurs; i++)
    {
        void *ret;
        pthread_join(consommateurs[i], &ret);
        if (ret == PTHREAD_CANCELED)
            printf("[Consommateur %d] annulé\n", ids_prod[i].id);
    }

    // Nettoyage
    sem_destroy(&places_libres);
    sem_destroy(&elements_disponibles);
    pthread_mutex_destroy(&mutex);
}

int main(void) {
    srand(time(NULL));

    init_semaphores();

    printf("=== Producteur-Consommateur ===\n");
    printf("Taille buffer : %d\n\n", TAILLE_BUFFER);

    // Scénario : 2 producteurs × 10 = 20 produits
    //            3 consommateurs × 7 = 21 demandes
    // → 1 consommateur va bloquer indéfiniment !
    printf("Consommateurs : %d, Producteurs : %d\n", 3, 2);
    do_test(2, 3, 10, 7);
    sleep(10);

    // Scénario 1 : Équilibré (production = consommation)
    // 3 producteurs × 10 = 30 produits
    // 5 consommateurs × 6 = 30 consommations
    // → Aucun blocage
    printf("Consommateurs : %d, Producteurs : %d\n", 5, 3);
    do_test(3, 5, 10, 6);
    sleep(10);

    // Scénario 2 : Producteurs plus rapides
    // 5 producteurs × 10 = 50 produits
    // 2 consommateurs × 10 = 20 consommations
    // → 3 ou plus producteurs vont se bloquer
    printf("Consommateurs : %d, Producteurs : %d\n", 2, 5);
    do_test(5, 1, 10, 10);
    sleep(10);

    // Scénario 3 : Consommateurs plus rapides
    // 1 producteur × 50 = 50 produits
    // 10 consommateurs × 10 = 100 consommations
    // → 5 ou plus consommateurs vont se bloquer
    printf("Consommateurs : %d, Producteurs : %d\n", 10, 1);
    do_test(1, 10, 50, 10);
    sleep(10);

    return 0;
}