#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <signal.h>
#include <openssl/md5.h>

#define NUM_CHILDREN 10
#define NUM_NONCES 5

// Fonction pour calculer le hachage MD5
char *md5hash(char *str) {
    static char hash[1 + 2 * MD5_DIGEST_LENGTH] = {0};
    unsigned char md5[MD5_DIGEST_LENGTH] = {0};

    MD5((unsigned char *)str, strlen(str), md5);

    for (int i = 0; i < MD5_DIGEST_LENGTH; i++) {
        sprintf(hash + 2 * i, "%02x", md5[i]);
    }
    return hash;
}

// Fonction pour vérifier si une chaîne commence par n caractères '0'
int zeros(char *s, int n) {
    for (int i = 0; i < n; i++) {
        if (s[i] != '0') {
            return 0;
        }
    }
    return 1;
}

void bruteforce(int first, int step, int zero) {
    char buffer[32];
    for (int i = first; i >= 0; i += step) {
        sprintf(buffer, "%d", i);
        char *hash = md5hash(buffer);
        if (zeros(hash, zero)) {
            printf("Nonce trouvé par le processus %d : %d\n", getpid(), i);
            exit(0);
        }
    }
}

int main() {
    int pids[NUM_CHILDREN];

    for (int i = 0; i < NUM_CHILDREN; i++) {
        int pid = fork();
        if (pid == 0) {
            bruteforce(i, NUM_CHILDREN, NUM_NONCES);
            exit(1);
        } else {
            pids[i] = pid;
        }
    }

    int nonces_found = 0;
    while (nonces_found < NUM_NONCES) {
        wait(NULL);
        nonces_found++;
    }

    // Tue les processus enfants restants
    for (int i = 0; i < NUM_CHILDREN; i++) {
        kill(pids[i], SIGTERM);
    }

    // Attend la fin des processus enfants
    for (int i = 0; i < NUM_CHILDREN; i++) {
        wait(NULL);
    }

    return 0;
}
