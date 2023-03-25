#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <sys/wait.h>


#define NUM_CHILDREN 10

// Fonction pour afficher une erreur et quitter le programme
void error(char *msg)
{
    perror(msg);
    exit(EXIT_FAILURE);
}

// Fonction de traitement du signal SIGTERM pour les processus enfants
void sigterm_handler(int signum)
{
    // Ne rien faire, on va simplement sortir de la pause de sleep()
}

int main()
{
    // Initialisation du générateur de nombres aléatoires
    srand(time(NULL));

    // Variables pour stocker les PID des processus enfants et leur numéro tiré au hasard
    pid_t child_pids[NUM_CHILDREN];
    int child_nums[NUM_CHILDREN];

    // Création des processus enfants
    for (int i = 0; i < NUM_CHILDREN; i++)
    {
        pid_t pid = fork();

        if (pid == -1)
        {
            error("Erreur lors de la création d'un processus enfant");
        }
        else if (pid == 0)
        {
            // Processus enfant : tirage d'un nombre aléatoire, affichage et attente de signaux SIGTERM

            // Tirage du nombre aléatoire
            int num = rand() % 10 + 1;
            child_nums[i] = num;

            // Affichage du PID et du nombre tiré
            printf("Processus enfant %d : PID = %d, Numéro tiré = %d\n", i+1, getpid(), num);

            // Installation du handler pour le signal SIGTERM
            struct sigaction sa;
            sa.sa_handler = sigterm_handler;
            if (sigaction(SIGTERM, &sa, NULL) == -1)
            {
                error("Erreur lors de l'installation du handler pour le signal SIGTERM");
            }

            // Pause pour attendre les signaux SIGTERM
            pause();

            // Terminaison avec le code de sortie 0
            exit(EXIT_SUCCESS);
        }
        else
        {
            // Processus parent : stockage du PID de l'enfant
            child_pids[i] = pid;
        }
    }

    // Attente de 1 seconde pour laisser le temps aux processus enfants de s'initialiser
    sleep(1);

    // Envoi des signaux SIGTERM pour deviner le numéro tiré par chaque enfant
    for (int i = 0; i < NUM_CHILDREN; i++)
    {
        int num = child_nums[i];
        for (int j = 0; j < num; j++)
        {
            kill(child_pids[i], SIGTERM);
        }
    }

    // Attente de la terminaison des processus enfants
    for (int i = 0; i < NUM_CHILDREN; i++)
    {
        wait(NULL);
    }

    return 0;
}
