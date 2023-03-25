#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <time.h>
#include <stdbool.h>

#define SHM_SIZE sizeof(int) + sizeof(int[100])

// Fonction pour afficher une erreur et quitter le programme
void error(char *msg)
{
    perror(msg);
    exit(EXIT_FAILURE);
}

// Fonction pour vérifier si un entier est premier
bool is_prime(int n)
{
    if (n < 2)
    {
        return false;
    }

    for (int i = 2; i * i <= n; i++)
    {
        if (n % i == 0)
        {
            return false;
        }
    }

    return true;
}

// Fonction pour trier un tableau d'entiers en ordre croissant
void sort_array(int arr[], int size)
{
    for (int i = 0; i < size - 1; i++)
    {
        for (int j = i + 1; j < size; j++)
        {
            if (arr[i] > arr[j])
            {
                int temp = arr[i];
                arr[i] = arr[j];
                arr[j] = temp;
            }
        }
    }
}

int main()
{
    // Génération de la clé pour le segment de mémoire partagée
    key_t key = ftok(".", 'S');
    if (key == -1)
    {
        error("Erreur lors de la génération de la clé pour le segment de mémoire partagée");
    }

    // Création du segment de mémoire partagée
    int shmid = shmget(key, SHM_SIZE, IPC_CREAT | 0666);
    if (shmid == -1)
    {
        error("Erreur lors de la création du segment de mémoire partagée");
    }

    // Attachement au segment de mémoire partagée
    int *shmaddr = shmat(shmid, NULL, 0);
    if (shmaddr == (void *)-1)
    {
        error("Erreur lors de l'attachement au segment de mémoire partagée");
    }

    // Initialisation du compteur et du tableau à 0
    int *counter = shmaddr;
    int *array = shmaddr + sizeof(int);

    *counter = 0;
    for (int i = 0; i < 100; i++)
    {
        array[i] = 0;
    }

    // Création de 10 processus enfants
    pid_t pid;
    for (int i = 0; i < 10; i++)
    {
        pid = fork();

        if (pid == -1)
        {
            error("Erreur lors de la création du processus enfant");
        }
        else if (pid == 0)
        {
            // Processus enfant : génération de 10 entiers aléatoires, ajout de ceux qui sont premiers et non déjà dans le tableau, et tri du tableau

            // Attachement au segment de mémoire partagée
            int *counter_child = shmaddr;
            int *array_child = shmaddr + sizeof(int);

            // Génération des entiers aléatoires
            srand(time(NULL) + getpid());
            for (int j = 0; j < 10; j++)
            {
                int n = rand() % 99 + 2;
                if (is_prime(n))
                {
                    bool already_in_array = false;

                    for (int k = 0; k < *counter_child; k++)
                    {
                        if (array_child[k] == n)
                        {
                            already_in_array = true;
                            break;
                        }
                    }

                    if (!already_in_array)
                    {
                        array_child[*counter_child] = n;
                        (*counter_child)++;
                    }
                }
            }

            // Tri du tableau en ordre croissant
            sort_array(array_child, *counter_child);

            // Détachement du segment de mémoire partagée
            if (shmdt(shmaddr) == -1)
            {
                error("Erreur lors du détachement du segment de mémoire partagée");
            }

            exit(EXIT_SUCCESS);
        }
    }

    // Attente de la terminaison de tous les enfants
    for (int i = 0; i < 10; i++)
    {
        if (wait(NULL) == -1)
        {
            error("Erreur lors de l'attente de la terminaison des processus enfants");
        }
    }

    // Affichage du contenu du tableau
    printf("Contenu du tableau :\n");
    for (int i = 0; i < *counter; i++)
    {
        printf("%d ", array[i]);
    }
    printf("\n");

    // Vérification que chaque élément du tableau est strictement inférieur au précédent
    for (int i = 1; i < *counter; i++)
    {
        if (array[i] <= array[i - 1])
        {
            printf("Erreur : le tableau n'est pas trié en ordre croissant.\n");
            break;
        }
    }

    // Détachement du segment de mémoire partagée
    if (shmdt(shmaddr) == -1)
    {
        error("Erreur lors du détachement du segment de mémoire partagée");
    }

    // Suppression du segment de mémoire partagée
    if (shmctl(shmid, IPC_RMID, NULL) == -1)
    {
        error("Erreur lors de la suppression du segment de mémoire partagée");
    }

    return 0;
}