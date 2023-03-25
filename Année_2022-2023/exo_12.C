#define _GNU_SOURCE
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Vérifie si un chemin donné correspond à un répertoire
int is_dir(char *path) {
    struct stat path_stat;
    stat(path, &path_stat);
    return S_ISDIR(path_stat.st_mode);
}

// Renvoie une structure contenant la liste des fichiers dans un répertoire
typedef struct dir_t {
    int count;
    char **names;
} dir_t;

dir_t ls(char *path) {
    DIR *dir;
    struct dirent *entry;
    dir_t result;
    result.count = 0;
    result.names = NULL;

    dir = opendir(path);
    if (dir == NULL) {
        perror("opendir");
        exit(EXIT_FAILURE);
    }

    // Parcours les fichiers du répertoire, ignore "." et ".."
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }
        // Ajoute le nom du fichier à la liste
        result.names = realloc(result.names, sizeof(char *) * (result.count + 1));
        result.names[result.count++] = strdup(entry->d_name);
    }

    closedir(dir);
    return result;
}

// Affiche le nom d'un fichier dans l'arborescence, avec un préfixe et une indication de fin de branche
void print_name(char *path, char *prefix, int last) {
    printf("%s", prefix);
    printf(last ? "\u2514" : "\u251c");
    printf("\u2500 ");
    printf("%s\n", path);
}

// Parcours récursivement l'arborescence d'un répertoire et affiche tous les fichiers
void tree(char *path, char *prefix, int last) {
    dir_t dir = ls(path);
    chdir(path);

    for (int i = 0; i < dir.count; ++i) {
        int new_last = (i == dir.count - 1);
        char *new_prefix;

        print_name(dir.names[i], prefix, new_last);

        if (is_dir(dir.names[i])) {
            // Construit le préfixe pour le sous-répertoire
            asprintf(&new_prefix, "%s%s  ", prefix, last ? " " : "\u2502");
            // Parcourt le sous-répertoire
            tree(dir.names[i], new_prefix, new_last);
            free(new_prefix);
            chdir("..");
        }

        free(dir.names[i]);
    }

    free(dir.names);
}

int main() {
    char prefix[] = "";
    tree(".", prefix, 1);
    return 0;
}
