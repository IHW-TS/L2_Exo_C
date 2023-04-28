// ldd: -lcrypto
// #define OPENSSL_SUPPRESS_DEPRECATED
#include <openssl/md5.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

// = {0} => all the array is reset to zero (only works for zero!)
char hash[1 + 2 * MD5_DIGEST_LENGTH] = {0};

// Calcule le hash MD5 de la chaîne 'str'
char *md5hash(char *str) {
    unsigned char md5[MD5_DIGEST_LENGTH] = {0};
    MD5((unsigned char *)str, strlen(str), md5);
    for (int i = 0; i < MD5_DIGEST_LENGTH; i++) {
        sprintf(hash + 2 * i, "%02x", md5[i]);
    }
    return hash;
}

// Vérifie si la chaîne 's' commence par 'n' caractères '0'
int zeros(char *s, int n) {
    for (int i = 0; s[i] && i < n; i++) {
        if (s[i] != '0') {
            return 0;
        }
    }
    return 1;
}

// Effectue un bruteforce pour trouver un entier dont le hachage MD5 commence par 'zero' caractères '0'
void bruteforce(int first, int step, int zero) {
    unsigned long long i;
    char s[128];
    int out;
    for (i = first; i += step;) {
        sprintf(s, "%llu", i);
        md5hash(s);
        if (zeros(hash, zero)) {
            break;
        }

    }
    sprintf(s, "found.%i", getpid());
    if ((out = open(s, O_WRONLY | O_CREAT | O_TRUNC, 0600)) == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }
    if (write(out, &i, sizeof(unsigned long long)) < sizeof(unsigned long long)) {
        perror("write");
        exit(EXIT_FAILURE);
    }

    if (close(out) == -1) {
        perror("close");
        exit(EXIT_FAILURE);
    }
    exit(0);
}

int main(void) {
    pid_t child, grp;
    int status, i, log;
    char path[128];
    unsigned long long winner;
    // Lancement de 10 processus enfants pour répartir le travail de bruteforce
    for (i = 0; i < 10; i++) {
        child = fork();
        if (child == -1) {
            perror("fork");
        } else if (child == 0) {
            bruteforce(i, 10, 6);
        } else {
            if (i == 0) {
                grp = child;
            }
            if (setpgid(child, grp) == -1) {
                perror("setpgid");
            }
        }
    }

    // Attendre le premier enfant qui termine avec le code de retour 0
    while (1) {
        child = wait(&status);
        if (child == -1) {
            perror("wait");
            if (errno == ECHILD) {
                exit(EXIT_FAILURE);
            }
        } else if (WIFEXITED(status) && WEXITSTATUS(status) == 0)
        {
            break; // On sort de la boucle quand un enfant a terminé avec le code de retour 0
        }
    }

    // Tuer tous les autres processus enfants
    kill(-grp, SIGKILL);

    // Lire le fichier found.PID correspondant à l'enfant qui a terminé
    sprintf(path, "found.%i", child);
    if ((log = open(path, O_RDONLY)) == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    // Lire la valeur de l'entier enregistré dans le fichier
    if (read(log, &winner, sizeof(unsigned long long)) < sizeof(unsigned long long)) {
        perror("read");
        exit(EXIT_FAILURE);
    }

    // Fermer le fichier
    if (close(log) == -1) {
        perror("close");
        exit(EXIT_FAILURE);
    }

    // Supprimer le fichier found.PID
    if (unlink(path) == -1) {
        perror("unlink");
        exit(EXIT_FAILURE);
    }

    // Afficher la valeur de l'entier trouvé
    printf("%llu\n", winner);

    return 0;
}
