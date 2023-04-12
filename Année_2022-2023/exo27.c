#include <openssl/md5.h> // inclusion de la librairie OpenSSL pour utiliser la fonction MD5
#include <string.h> // inclusion de la librairie pour utiliser la fonction strlen
#include <stdio.h> // inclusion de la librairie standard d'entrée/sortie
#include <pthread.h> // inclusion de la librairie pour utiliser les threads
#include <stdlib.h> // inclusion de la librairie standard pour la gestion de la mémoire

#define NUM_THREADS 10 // définition d'une constante pour le nombre de threads à créer
#define NUM_NONCES 5 // définition d'une constante pour le nombre de "nonces" à trouver

char hash[1 + 2*MD5_DIGEST_LENGTH] = {0}; // tableau pour stocker le résultat du hash MD5 en hexadécimal

// fonction qui calcule le hash MD5 d'une chaîne de caractères et retourne le résultat en hexadécimal
char* md5hash(char *str) {
    unsigned char md5[MD5_DIGEST_LENGTH] = {0}; // tableau pour stocker le résultat du hash MD5
    MD5((unsigned char*)str, strlen(str), md5); // appel de la fonction MD5 pour calculer le hash
    for (int i = 0; i < MD5_DIGEST_LENGTH; i++) { // parcours du tableau de résultats MD5
        sprintf(hash + 2*i, "%02x", md5[i]); // conversion des résultats en hexadécimal et stockage dans le tableau "hash"
    }
    return hash; // retourne le tableau "hash" contenant le résultat du hash MD5 en hexadécimal
}

// fonction qui vérifie si une chaîne de caractères commence par "n" zéros
int zeros(char *s, int n) {
    for (int i = 0; i < n; i++) { // parcours des "n" premiers caractères de la chaîne
        if (s[i] != '0') { // si un caractère est différent de '0'
            return 0; // retourne 0 (faux)
        }
    }
    return 1; // retourne 1 (vrai) si tous les premiers caractères sont '0'
}

// structure de données pour stocker les informations d'un thread
typedef struct {
    int first; // premier nombre à tester
    int step; // intervalle entre chaque nombre à tester
    int zero; // nombre de zéros à chercher
    int nonces[NUM_NONCES]; // tableau pour stocker les "nonces" trouvés par le thread
} ThreadData;

// fonction exécutée par chaque thread pour trouver des "nonces"
void *bruteforce(void *arg) {
    ThreadData *data = (ThreadData *)arg; // récupération des données du thread
    int nonce_found = 0; // nombre de "nonces" trouvés par le thread
    char buffer[64]; // buffer pour stocker le nombre à tester sous forme de chaîne de caractères

    for (int i = data->first; nonce_found < NUM_NONCES; i += data->step) { // boucle pour tester les nombres
        sprintf(buffer, "%d", i); // conversion du nombre en chaîne de caractères
        char *hashed = md5hash(buffer); // calcul du hash MD5 de la chaîne de caractères

        if (zeros(hashed, data->zero)) { // si le hash commence par "data->zero" zéros
            data->nonces[nonce_found] = i; // stocke le nombre trouvé dans le tableau "nonces"
            nonce_found++; // incrémente le nombre de "nonces" trouvés
        }
    }
    return NULL; // termine le thread
}

int main() {
    pthread_t threads[NUM_THREADS]; // tableau pour stocker les identifiants des threads
    ThreadData thread_data[NUM_THREADS]; // tableau pour stocker les données des threads
    int zero = 6; // nombre de zéros à chercher

    for (int i = 0; i < NUM_THREADS; i++) { // boucle pour créer les threads
        thread_data[i].first = i; // premier nombre à tester par le thread
        thread_data[i].step = NUM_THREADS; // intervalle entre chaque nombre à tester par le thread
        thread_data[i].zero = zero; // nombre de zéros à chercher par le thread

        pthread_create(&threads[i], NULL, bruteforce, (void *)&thread_data[i]); // création du thread avec les données correspondantes
    }
    for (int i = 0; i < NUM_THREADS; i++) { // boucle pour attendre la fin de chaque thread
        pthread_join(threads[i], NULL);
    }
    for (int i = 0; i < NUM_THREADS; i++) { // boucle pour afficher les "nonces" trouvés par chaque thread
        printf("Thread %d nonces:\n", i);
        for (int j = 0; j < NUM_NONCES; j++) {
            printf("%d\n", thread_data[i].nonces[j]); // affiche chaque "nonce" trouvé par le thread i
        }
    }

    return 0; // termine le programme
}



