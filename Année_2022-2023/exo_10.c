#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[], char *envp[]) {
    char *cmd = NULL;
    int cmd_start = 0;

    for (int i = 1; i < argc; i++) {
        char *equals_sign = strchr(argv[i], '=');

        if (equals_sign != NULL) {
            // C'est une variable d'environnement (de la forme NOM=VALEUR ou NOM=).
            *equals_sign = '\0';
            if (*(equals_sign + 1) == '\0') {
                // Supprimer la variable d'environnement NOM.
                unsetenv(argv[i]);
            } else {
                // Positionner la variable d'environnement NOM à VALEUR.
                setenv(argv[i], equals_sign + 1, 1);
            }
            *equals_sign = '=';
        } else {
            // C'est une chaîne à concaténer dans cmd.
            if (cmd_start == 0) {
                cmd_start = i;
            }
        }
    }

    if (cmd_start > 0) {
        // Concaténer les chaînes de la ligne de commande dans cmd.
        for (int i = cmd_start; i < argc; i++) {
            int new_cmd_len = (cmd ? strlen(cmd) : 0) + 1 + strlen(argv[i]) + 1;
            char *new_cmd = realloc(cmd, new_cmd_len);
            if (new_cmd == NULL) {
                perror("realloc");
                free(cmd);
                return EXIT_FAILURE;
            }
            cmd = new_cmd;
            strcat(cmd, " ");
            strcat(cmd, argv[i]);
        }

        // Appeler system(cmd).
        int result = system(cmd);
        free(cmd);
        return result;
    }

    return EXIT_SUCCESS;
}
