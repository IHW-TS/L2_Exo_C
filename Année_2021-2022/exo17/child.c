
#include <signal.h> 
#include <stdio.h> 
#include <stdlib.h>
#include <unistd.h> 

// initialisation de la variable num
int num, nbSig = 0;

// récupération du signal sigint
void actionSignal() {
    if(nbSig < num-1) {
        nbSig++;
        printf("Fils de pid %d dit : SIGTERM reçu\n");
        signal(SIGTERM, actionSignal);
    }
    else {
        // Le fils se termine
        if(num == 1 || num == 0) {
            printf("Fils de pid : %d s'est fini en %d signal\n", getpid(), num);
        }
        else {
            printf("Fils de pid : %d s'est fini en %d signaux\n", getpid(), num);
        }

        exit(0);
    }
}

void construct() {
    srand(getpid());// srand va permettre d'eviter de generer deux fois le meme pid
    num = rand() % 10;
    printf("Fils de pid : %d et de variable num : %d\n", getpid(), num);

    while(1) {
        signal(SIGTERM, actionSignal);
    }
}

int main(int argc, char const *argv[])
{
    construct();
    return 0;
}
