#include <stdio.h>          // Inclut la bibliothèque standard d'entrée/sortie
#include <unistd.h>         // Inclut la bibliothèque pour les fonctions système POSIX
#include <signal.h>         // Inclut la bibliothèque pour la gestion des signaux
#include <stdbool.h>        // Inclut la bibliothèque pour les types booléens
#include <time.h>           // Inclut la bibliothèque pour les fonctions de manipulation du temps

volatile bool ctrl_c_pressed = false;                    // Variable booléenne qui indique si l'utilisateur a appuyé sur Ctrl+C
volatile bool waiting_for_second_ctrl_c = false;         // Variable booléenne qui indique si le programme attend une seconde pression sur Ctrl+C

void handle_signal(int sig) {
    if (waiting_for_second_ctrl_c) {                     // Si le programme attend une seconde pression sur Ctrl+C
        printf("\nAu revoir !\n");                       // Affiche un message d'adieu
        exit(0);                                         // Termine le programme
    } else {
        ctrl_c_pressed = true;                           // Enregistre que l'utilisateur a appuyé sur Ctrl+C
    }
}

void alarm_handler(int sig) {
    waiting_for_second_ctrl_c = false;                   // Met à jour la variable qui indique si le programme attend une seconde pression sur Ctrl+C
}
int main() {
    signal(SIGINT, handle_signal);                       // Enregistre la fonction handle_signal() comme fonction de traitement pour le signal SIGINT (Ctrl+C)
    signal(SIGALRM, alarm_handler);                      // Enregistre la fonction alarm_handler() comme fonction de traitement pour le signal SIGALRM

    while (1) {
        if (ctrl_c_pressed) {                            // Si l'utilisateur a appuyé sur Ctrl+C
            printf("\nTapez Ctrl+C une deuxième fois dans les deux secondes pour terminer le programme.\n");  // Affiche un message demandant à l'utilisateur d'appuyer sur Ctrl+C une seconde fois pour terminer le programme
            ctrl_c_pressed = false;                      // Réinitialise la variable qui indique si l'utilisateur a appuyé sur Ctrl+C
            waiting_for_second_ctrl_c = true;             // Enregistre que le programme attend une seconde pression sur Ctrl+C
            alarm(2);                                     // Déclenche une alarme de deux secondes
        } else if (!waiting_for_second_ctrl_c) {          // Si le programme n'attend pas une seconde pression sur Ctrl+C
            printf("working...\n");                       // Affiche un message pour indiquer que le programme est en train de travailler
            sleep(1);                                     // Attend une seconde
        }
    }

    return 0;
}