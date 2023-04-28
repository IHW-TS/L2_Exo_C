#include <stdio.h>      // Inclut la bibliothèque standard pour les entrées/sorties en C.
#include <stdlib.h>     // Inclut la bibliothèque standard pour les fonctions générales en C.
#include <sys/types.h>  // Inclut les définitions de types de données standard en C pour les appels système.
#include <sys/stat.h>   // Inclut les définitions de types de données pour la structure stat en C.
#include <unistd.h>     // Inclut les définitions de types de données standard en C pour les fonctions de système d'exploitation.
#include <dirent.h>     // Inclut les définitions de types de données pour la structure dirent en C.
#include <string.h>     // Inclut les définitions pour les fonctions de manipulation de chaînes de caractères en C.
#include <pwd.h>        // Inclut les définitions de types de données pour la structure passwd en C.
#include <grp.h>        // Inclut les définitions de types de données pour la structure group en C.
#include <time.h>       // Inclut les définitions pour la manipulation de temps en C.
#include <stdbool.h>    // Inclut les définitions de type de données booléennes en C.

// Compare deux dirent par leur nom.
int compare_dirent(const void *a, const void *b) {
    // Convertit les paramètres a et b en pointeurs vers des dirent.
    struct dirent *dir_a = *(struct dirent **)a;
    struct dirent *dir_b = *(struct dirent **)b;
    // Compare les noms des dirent et retourne la différence.
    return strcmp(dir_a->d_name, dir_b->d_name);
}

// Affiche les informations de fichier.
void display_file_info(struct dirent *entry, struct stat *statbuf) {
    // Tableau de caractères représentant les droits du fichier.
    char mode[] = "----------";

    // Détermine le type de fichier et le stocke dans le premier élément de la chaîne de caractères "mode".
    if (S_ISDIR(statbuf->st_mode)) mode[0] = 'd';
    else if (S_ISLNK(statbuf->st_mode)) mode[0] = 'l';
    else if (S_ISFIFO(statbuf->st_mode)) mode[0] = 'p';
    else if (S_ISSOCK(statbuf->st_mode)) mode[0] = 's';
    else if (S_ISBLK(statbuf->st_mode)) mode[0] = 'b';
    else if (S_ISCHR(statbuf->st_mode)) mode[0] = 'c';

    // Détermine les permissions et les stocke dans les éléments suivants de la chaîne de caractères "mode".
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
    // Obtenir les informations de temps dans un format structuré.
    struct tm *timeinfo = localtime(&(statbuf->st_mtime));
    // Créer une chaîne de caractères pour stocker les informations de temps.
    char time_str[20];
    // Formater les informations de temps dans la chaîne de caractères "time_str".
    strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M", timeinfo);

    // Afficher les informations de fichier.
    printf("%s %2lu %5d %5d %8ld %s %s",
           mode, statbuf->st_nlink, statbuf->st_uid, statbuf->st_gid, statbuf->st_size, time_str, entry->d_name);

    // Si le fichier est un lien symbolique, afficher sa cible.
    if (S_ISLNK(statbuf->st_mode)) {
        // Créer un tampon pour stocker la cible du lien symbolique.
        char link_target[256];
        // Lire la cible du lien symbolique dans le tampon.
        ssize_t len = readlink(entry->d_name, link_target, sizeof(link_target) - 1);
        // Si la lecture est réussie, ajouter la cible à l'affichage.
        if (len != -1) {
            link_target[len] = '\0';
            printf(" -> %s", link_target);
        }
    }
    // Aller à la ligne suivante.
    printf("\n");
}

// Définition de la fonction principale.
int main() {
    // Déclaration des variables nécessaires.
    DIR *dirp;                  // Pointeur vers la structure représentant un répertoire.
    struct dirent *entry;       // Pointeur vers la structure représentant une entrée dans le répertoire.
    struct stat statbuf;        // Structure pour stocker les informations de stat sur une entrée dans le répertoire.

    // Ouvrir le répertoire courant ".".
    dirp = opendir(".");
    // Vérifier si l'ouverture a réussi. Si non, afficher une erreur et quitter le programme.
    if (dirp == NULL) {
        perror("opendir");
        return EXIT_FAILURE;
    }

    // Compter le nombre d'entrées dans le répertoire et les stocker dans un tableau.
    size_t entry_count = 0;
    struct dirent **entries = NULL;
    while ((entry = readdir(dirp)) != NULL) {
        // Augmenter la taille du tableau d'entrées.
        entries = realloc(entries, (entry_count + 1) * sizeof(struct dirent *));
        // Vérifier si la réallocation a réussi. Si non, afficher une erreur, libérer la mémoire et fermer le répertoire.
        if (entries == NULL) {
            perror("realloc");
            closedir(dirp);
            return EXIT_FAILURE;
        }
        // Ajouter l'entrée au tableau d'entrées.
        entries[entry_count] = entry;
        entry_count++;
    }

    // Trier les entrées par ordre alphabétique.
    qsort(entries, entry_count, sizeof(struct dirent *), compare_dirent);

    // Afficher les informations de chaque entrée.
    for (size_t i = 0; i < entry_count; i++) {
        // Obtenir l'entrée courante.
        entry = entries[i];
        // Obtenir les informations de stat pour l'entrée.
        if (lstat(entry->d_name, &statbuf) == -1) {
            perror("lstat");
            continue;
        }
        // Afficher les informations de fichier pour l'entrée.
        display_file_info(entry, &statbuf);
    }

    // Libérer la mémoire allouée pour le tableau d'entrées et fermer le répertoire.
    free(entries);
    closedir(dirp);

    // Retourner une valeur de sortie réussie.
    return EXIT_SUCCESS;
}

