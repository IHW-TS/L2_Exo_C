#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <time.h>

#define NUM_CHILDREN 10

// Tableau pour stocker le comportement choisi par chaque enfant
int child_behaviors[NUM_CHILDREN];

// Fonction pour afficher une erreur et quitter le programme
void error(char *msg)
{
    perror(msg);
    exit(EXIT_FAILURE);
}

// Fonction pour le traitement du signal SIGUSR1
void sigusr1_handler(int sig)
{
    // Ne fait rien
}

int main()
{
    // Initialisation du générateur de nombres aléatoires
    srand(time(NULL));

    // Démarrage des processus enfants
    pid_t pid;
    for (int i = 0; i < NUM_CHILDREN; i++)
    {
        pid = fork();

        if (pid == -1)
        {
            error("Erreur lors de la création du processus enfant");
        }
        else if (pid == 0)
        {
            // Processus enfant : choix aléatoire du comportement
            int behavior = rand() % 4;
            child_behaviors[i] = behavior;
            switch (behavior)
            {
                case 0:
                    // Dort indéfiniment
                    while (1)
                    {
                        sleep(1);
                    }
                    break;
                case 1:
                    // Ignore SIGUSR1 indéfiniment
                    signal(SIGUSR1, SIG_IGN);
                    while (1)
                    {
                        pause();
                    }
                    break;
                case 2:
                    // Termine avec le code de sortie 0 à la réception de SIGUSR1
                    signal(SIGUSR1, sigusr1_handler);
                    pause();
                    exit(0);
                    break;
                case 3:
                    // Termine avec le code de sortie 1 à la réception de SIGUSR1
                    signal(SIGUSR1, sigusr1_handler);
                    pause();
                    exit(1);
                    break;
                default:
                    error("Comportement d'enfant inconnu");
            }
        }
    }

    // Envoi du signal SIGUSR1 à tous les enfants pour les réveiller
    for (int i = 0; i < NUM_CHILDREN; i++)
    {
        if (kill(pid, SIGUSR1) == -1)
        {
            error("Erreur lors de l'envoi du signal SIGUSR1");
        }
    }

    // Attente de la terminaison de tous les enfants et affichage de leur comportement
    int status;
    for (int i = 0; i < NUM_CHILDREN; i++)
    {
        pid = wait(&status);
        if (WIFEXITED(status))
        {
            printf("Enfant %d : comportement %d, code de sortie %d\n", pid, child_behaviors[i], WEXITSTATUS(status));
        }
        else
        {
            printf("Enfant %d : comportement %d, terminé avec un signal\n", pid, child_behaviors[i]);
        }
    }

    return 0;
}
