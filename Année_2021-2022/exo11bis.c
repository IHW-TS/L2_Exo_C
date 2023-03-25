#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
/*#include <pwd.h>*/


void ls(char[]);
void stat_fichier(char *nom_du_fichier);
void list(char *nom_du_fichier, struct stat *statinfo);
void information(mode_t fichier, char str[]);
void temps(time_t temps_du_fichier);
/*
char* uid_to_name(uid_t uid);
char* gid_to_name(gid_t gid);
*/

int main(int argc, char *argv[])
{
    if (argc == 1) //alors aucun argument n'est passé en parametre 
    {
        ls(" . ");
    }
    else
    {
        while (--argc)
        {
            printf("arg=%s\n ", *++argv);
            ls(*argv);
        }
    }
    return 0;
}

void ls(char dir[])
{
    DIR *a;
    struct dirent *cur;
    if ((a = opendir(dir)) == NULL)
    {
        perror("Erreur"); // message sur la sortie d'erreur standard, décrivant la dernière erreur rencontrée durant un appel système ou une fonction de bibliothèque
        exit(-1);
    }
    else
    {
        while ((cur = readdir(a)) != NULL)
        {
            stat_fichier(cur->d_name);
        }
        closedir(a);

    }
}

// Récupère les informations sur le fichier

void stat_fichier(char *nom_du_fichier)
{
    struct stat st;
    if (stat(nom_du_fichier, &st) == -1)
    {
        printf(" open%s fail\n ", nom_du_fichier);
        exit(-1);
        
    }
    else
    {
        list(nom_du_fichier, &st);
    }
}

// Afficher la liste des fichiers

void list(char *nom_du_fichier, struct stat *statinfo)
{
    mode_t st_mode = statinfo->st_mode;

    char str[10];
    information(st_mode, str);
    printf("%s\t", str);
    /*
    printf("%s\t\t", uid_to_name( statinfo->st_uid ) ); //Nom D'utilisateu 
    printf("%s\t", gid_to_name( statinfo->st_gid ) ); //Nom du Groupe
    */
    printf("%ld\t", statinfo->st_nlink); // Lien symbolique
    printf("%10ld", statinfo->st_size);  // taille du fichier
    temps(statinfo->st_mtime);           // Heure de la dernière modification
    printf("\t%s", nom_du_fichier);      // Nom fichier
    printf("\n");                        // Saut de ligne 
}

void information(mode_t fichier, char str[])
{

    strcpy(str, "----------"); // va permettre de copier la chaine de caratere dans une autre
    if (S_ISREG(fichier))
        str[0] = '-';
    if (S_ISDIR(fichier))
        str[0] = 'd';
   /* if (S_ISLNK (fichier))
        str[0] = 'l';*/
   /* if (S_ISSOCK(fichier))
        str[0] = 's';*/
    if (S_ISFIFO (fichier))
        str[0] = 'p';
    if (S_ISBLK (fichier))
        str[0] = 'b';
    if (S_ISCHR (fichier))
        str[0] = 'c';

/*
S_ISUID
S_ISGID
S_ISVTX

*/

    if (fichier & S_IRUSR)
        str[1] = 'r';
    if (fichier & S_IWUSR)
        str[2] = 'w';
    if (fichier & S_IXUSR)
        str[3] = 'x';

    if (fichier & S_IRGRP)
        str[4] = 'r';
    if (fichier & S_IWGRP)
        str[5] = 'w';
    if (fichier & S_IXGRP)
        str[6] = 'x';

    if (fichier & S_IROTH)
        str[7] = 'r';
    if (fichier & S_IWOTH)
        str[8] = 'w';
    if (fichier & S_IXOTH)
        str[9] = 'x';
}
void temps(time_t temps_du_fichier)
{

    struct tm *local = localtime(&temps_du_fichier);
    int h = local->tm_hour;
    int min = local->tm_min;
    int s = local->tm_sec;
    int day = local->tm_mday;
    int mois = local->tm_mon + 1;
    int an = local->tm_year + 1900;

    printf(" %02d/%02d/%d", day, mois, an);
    printf(" %02d:%02d:%02d", h, min, s);
}
