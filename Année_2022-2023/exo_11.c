#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <stdbool.h>

// Compare deux dirent par leur nom.
int compare_dirent(const void *a, const void *b) {
    struct dirent *dir_a = *(struct dirent **)a;
    struct dirent *dir_b = *(struct dirent **)b;
    return strcmp(dir_a->d_name, dir_b->d_name);
}

// Affiche les informations de fichier.
void display_file_info(struct dirent *entry, struct stat *statbuf) {
    char mode[] = "----------";

    // Type de fichier
    if (S_ISDIR(statbuf->st_mode)) mode[0] = 'd';
    else if (S_ISLNK(statbuf->st_mode)) mode[0] = 'l';
    else if (S_ISFIFO(statbuf->st_mode)) mode[0] = 'p';
    else if (S_ISSOCK(statbuf->st_mode)) mode[0] = 's';
    else if (S_ISBLK(statbuf->st_mode)) mode[0] = 'b';
    else if (S_ISCHR(statbuf->st_mode)) mode[0] = 'c';

    // Permissions
    for (int i = 0; i < 3; i++) {
        int shift = 6 - i * 3;
        if (statbuf->st_mode & (S_IRUSR >> i * 3)) mode[1 + i * 3] = 'r';
        if (statbuf->st_mode & (S_IWUSR >> i * 3)) mode[2 + i * 3] = 'w';
        if (statbuf->st_mode & (S_IXUSR >> i * 3)) mode[3 + i * 3] = 'x';
    }

    // Bits spéciaux
    if (statbuf->st_mode & S_ISUID) mode[3] = (mode[3] == 'x') ? 's' : 'S';
    if (statbuf->st_mode & S_ISGID) mode[6] = (mode[6] == 'x') ? 's' : 'S';
    if (statbuf->st_mode & S_ISVTX) mode[9] = (mode[9] == 'x') ? 't' : 'T';

    // Temps
    struct tm *timeinfo = localtime(&(statbuf->st_mtime));
    char time_str[20];
    strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M", timeinfo);

    printf("%s %2lu %5d %5d %8ld %s %s",
           mode, statbuf->st_nlink, statbuf->st_uid, statbuf->st_gid, statbuf->st_size, time_str, entry->d_name);

    if (S_ISLNK(statbuf->st_mode)) {
        char link_target[256];
        ssize_t len = readlink(entry->d_name, link_target, sizeof(link_target) - 1);
        if (len != -1) {
            link_target[len] = '\0';
            printf(" -> %s", link_target);
        }
    }
    printf("\n");
}

int main() {
    DIR *dirp;
    struct dirent *entry;
    struct stat statbuf;

    dirp =    opendir(".");
    if (dirp == NULL) {
        perror("opendir");
        return EXIT_FAILURE;
    }

    // Compter les entrées dans le répertoire et les stocker dans un tableau.
    size_t entry_count = 0;
    struct dirent **entries = NULL;
    while ((entry = readdir(dirp)) != NULL) {
        entries = realloc(entries, (entry_count + 1) * sizeof(struct dirent *));
        if (entries == NULL) {
            perror("realloc");
            closedir(dirp);
            return EXIT_FAILURE;
        }
        entries[entry_count] = entry;
        entry_count++;
    }

    // Trier les entrées par ordre alphabétique.
    qsort(entries, entry_count, sizeof(struct dirent *), compare_dirent);

    // Afficher les informations de chaque entrée.
    for (size_t i = 0; i < entry_count; i++) {
        entry = entries[i];
        if (lstat(entry->d_name, &statbuf) == -1) {
            perror("lstat");
            continue;
        }
        display_file_info(entry, &statbuf);
    }

    // Libérer la mémoire et fermer le répertoire.
    free(entries);
    closedir(dirp);

    return EXIT_SUCCESS;
}

