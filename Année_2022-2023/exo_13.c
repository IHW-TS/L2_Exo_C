#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Fonction pour copier les permissions du fichier source vers le fichier cible
void cp_mode(char *src, char *tgt) {
    struct stat src_stat;
    if (stat(src, &src_stat) == -1) {
        perror("stat"); // Affiche un message d'erreur sur stderr avec la description de l'erreur (si elle existe)
        exit(EXIT_FAILURE); // Termine le programme avec un code de retour correspondant à une erreur
    }

    if (chmod(tgt, src_stat.st_mode) == -1) {
        perror("chmod");
        exit(EXIT_FAILURE);
    }
}

// Fonction pour copier un fichier
void cp_file(char *src, char *tgt) {
    int src_fd, tgt_fd;
    char buffer[4096];
    ssize_t read_bytes, write_bytes;

    src_fd = open(src, O_RDONLY); // Ouvre le fichier source en lecture seulement
    if (src_fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    tgt_fd = open(tgt, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH); // Ouvre le fichier cible en écriture, le crée s'il n'existe pas, et donne les permissions correspondantes
    if (tgt_fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    while ((read_bytes = read(src_fd, buffer, sizeof(buffer))) > 0) { // Lit les données du fichier source en blocs de la taille du buffer, tant qu'il en reste à lire
        write_bytes = write(tgt_fd, buffer, read_bytes); // Écrit les données lues dans le fichier cible
        if (write_bytes != read_bytes) {
            perror("write");
            exit(EXIT_FAILURE);
        }
    }

    if (read_bytes == -1) {
        perror("read");
        exit(EXIT_FAILURE);
    }

    close(src_fd); // Ferme le fichier source
    close(tgt_fd); // Ferme le fichier cible

    cp_mode(src, tgt); // Copie les permissions du fichier source vers le fichier cible
}

// Fonction pour copier un répertoire
void cp_dir(char *src, char *tgt) {
    DIR *dir;
    struct dirent *entry;
    struct stat src_stat;
    char src_path[PATH_MAX], tgt_path[PATH_MAX];

    if (stat(src, &src_stat) == -1) {
        perror("stat");
        exit(EXIT_FAILURE);
    }

    if (mkdir(tgt, src_stat.st_mode) == -1) { // Crée le répertoire cible avec les mêmes permissions que le répertoire source
        perror("mkdir");
        exit(EXIT_FAILURE);
    }

    dir = opendir(src); // Ouvre le répertoire source
    if (dir == NULL) {
        perror("opendir");
        exit(EXIT_FAILURE);
    }

    while ((entry = readdir(dir)) != NULL) { // Lit les entrées du répertoire source
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        snprintf(src_path, sizeof(src_path), "%s/%s", src, entry->d_name);
        snprintf(tgt_path, sizeof(tgt_path), "%s/%s", tgt, entry->d_name);

        if (entry->d_type == DT_DIR) {
            cp_dir(src_path, tgt_path);
        } else {
            cp_file(src_path, tgt_path);
        }
    }

    closedir(dir);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <source> <target>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    struct stat src_stat;
    if (stat(argv[1], &src_stat) == -1) {
        perror("stat");
        exit(EXIT_FAILURE);
    }

    if (S_ISDIR(src_stat.st_mode)) {
        cp_dir(argv[1], argv[2]);
    } else {
        cp_file(argv[1], argv[2]);
    }

    return 0;
}
