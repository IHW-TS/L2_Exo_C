#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <glob.h>

int main() {
    glob_t glob_result; // création d'une structure pour stocker les résultats de la recherche de fichiers

    // recherche des fichiers avec l'extension .c
    glob("*.c", GLOB_TILDE, NULL, &glob_result);

    // allocation de la mémoire pour stocker les arguments de la commande ls
    char **args = malloc((4 + glob_result.gl_pathc) * sizeof(char *));
    args[0] = "ls"; // nom de la commande
    args[1] = "-l"; // option pour afficher les informations détaillées sur les fichiers

    // ajout des fichiers trouvés à la liste des arguments
    for (size_t i = 0; i < glob_result.gl_pathc; i++) {
        args[2 + i] = glob_result.gl_pathv[i];
    }

    args[2 + glob_result.gl_pathc] = NULL; // dernier argument doit être NULL


    int pipefd[2]; // déclaration du tableau pour stocker les descripteurs de fichiers des pipes (canal de communication entre deux processus)

    if (pipe(pipefd) == -1) { // gestion d'erreur
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    pid_t pid_ls = fork(); // création du processus fils pour la commande ls
    if (pid_ls == -1) { // gestion d'erreur
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid_ls == 0) { // si on est dans le processus fils pour la commande ls
        dup2(pipefd[1], STDOUT_FILENO); // redirige la sortie standard vers le pipe
        close(pipefd[0]); // ferme la lecture du pipe
        close(pipefd[1]); // ferme l'écriture du pipe
        execlp("ls", "ls", "-l", NULL); // exécute la commande ls -l
        perror("execlp"); // affiche une erreur si l'exécution a échoué
        exit(EXIT_FAILURE); 
    }

    pid_t pid_sed = fork(); // création du processus fils pour la commande sed
    if (pid_sed == -1) { // gestion d'erreur
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid_sed == 0) {
        dup2(pipefd[0], STDIN_FILENO);
        close(pipefd[0]);
        close(pipefd[1]);
        execlp("sed", "sed", "s/\\.c$/.COUCOU/", NULL);
        perror("execlp");
        exit(EXIT_FAILURE);
    }

    close(pipefd[0]);
    close(pipefd[1]);

    waitpid(pid_ls, NULL, 0);
    waitpid(pid_sed, NULL, 0);

    globfree(&glob_result);
    free(args);

    return 0;
}
