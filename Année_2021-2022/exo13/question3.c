
void cp_dir(char *src, char *tgt)
{
	struct stat tmp;

	char Nwsrc[10000];
	char Nwtgt[10000];

	if (mkdir(tgt, tmp.st_mode)) 
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
		if (fol > 2) 
			memset(Nwsrc,0,sizeof(Nwsrc));
			memset(Nwtgt,0,sizeof(Nwtgt));

			sprintf(Nwsrc, "%s/%s", src, b->d_name); 
			sprintf(Nwtgt, "%s/%s", tgt, b->d_name);

			if (b->d_type == DT_DIR)
				cp_dir(Nwsrc, Nwtgt);
			else
				cp_file(Nwsrc, Nwtgt);
		}
	}
}