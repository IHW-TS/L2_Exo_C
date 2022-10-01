#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define BUFFERSIZE 10000

int is_dir(char *path);
void cp_mode(char *src, char *tgt);
void cp_file(char *src, char *tgt);
void cp_dir(char *src, char *tgt);
void erreur(char *src, char *tgt);

int main(int argc, char *argv[])
// argv[1] = fichier source  argv[2] = fichier de destination
{
	if (argc < 3)
	{
		printf("Erreur Fichier !");
		return -1;
	}
	char *srcPath = argv[1];
	char *destPath = argv[2];

	if (is_dir(srcPath)) // copie du dossier
	{
		cp_dir(srcPath, destPath);
	}
	else
	{
		if (access(destPath, F_OK) == 0) // on assure que destPath est un répertoire qui n'existe pas
		{
			printf("le fichier source existe déjà, il sera donc écrasé !\n");
			cp_file(srcPath, destPath); // fichier a copier
		}
	}
	return 0;
}

void cp_mode(char *src, char *tgt)
{
	struct stat tmp;
	if (stat(src, &tmp) != 0) // on recupere les permissions du premier fichier
	{
		printf("Erreur Stat");
		exit(-1);
	}

	if (chmod(tgt, tmp.st_mode) != 0) // on les attribue via chmod au deuxieme fichier
									  // je pense que y a un truc a prescise a "chmod" pour lui preciser que les infomrations dans stat son en octal
	{
		printf("Erreur Stat");
		exit(-1);
	}
}

void cp_file(char *src, char *tgt)
{
	struct stat tmp;
	int s, t, chr;
	char buf[BUFFERSIZE];

	// Ouverture des fichier
	if ((s = open(src, O_RDONLY)) == -1) // Ouverture du premier fichier en lecteur -1 renvoie a une erreur
	{
		erreur("Erreur lors de l'ouverture !", src);
	}

	if ((t = open(tgt, O_WRONLY | O_CREAT | O_TRUNC, tmp.st_mode)) == -1) // Ouverture du second fichier avec les autorisations du premier
	{
		erreur("Erreur lors de la création !", tgt);
	}

	// copy files 
	while ((chr = read(s, buf, BUFFERSIZE)) > 0)
	{
		if (write(t, buf, chr) != chr)
		{
			erreur("Erreur d'ecriture !", tgt);
		}

		if (chr == -1)
		{
			erreur("Erreur de lecture !", src);
		}
	}

	// close files 
	if (close(s) == -1 || close(t) == -1)
	{
		printf("Erreur fermeture des fichiers !");
		exit(EXIT_FAILURE);
	}
}

int mkdir(const char *pathname, mode_t mode);

void cp_dir(char *src, char *tgt)
{
	struct stat tmp;

	char Nwsrc[10000];
	char Nwtgt[10000];

	if (mkdir(tgt, tmp.st_mode)) // Si il n'existe pas on utilise la fonction mkdir pour le crée
	{
		printf("Le fichier existe deja !\n");
		exit(EXIT_FAILURE);
	}

	DIR *a = opendir(src);
	if (a == NULL)
	{
		printf("Erreur lors de l'ouverture de ce fichier = %s\n", src);
		exit(EXIT_FAILURE);
	}
	struct dirent *b = NULL;
	int fol = 0;
	while ((b = readdir(a)))
	{
		fol++;
		if (fol > 2) // surpprime les fichiers cachés
		{
			memset(Nwsrc,0,sizeof(Nwsrc));
			memset(Nwtgt,0,sizeof(Nwtgt));

			sprintf(Nwsrc, "%s/%s", src, b->d_name); // sauvegarde le nouveau fichier dans le path
			sprintf(Nwtgt, "%s/%s", tgt, b->d_name);

			if (b->d_type == DT_DIR)
				cp_dir(Nwsrc, Nwtgt);
			else
				cp_file(Nwsrc, Nwtgt);
		}
	}
}

int is_dir(char *path)
{
	struct stat st;
	stat(path, &st);
	if (S_ISDIR(st.st_mode))
		return 1;
	else
		return 0;
}

void erreur(char *src, char *tgt)
{
	fprintf(stderr, "Erreur: %s ", src);
	perror(tgt);
	exit(1);
}

/*
if (src == NULL || tgt == NULL)
{
printf("Erreur regarder si vous avez les auroisations de modification sur les fichiers.\n");

exit(EXIT_FAILURE);
}
*/