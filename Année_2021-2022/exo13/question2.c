
void cp_file(char *src, char *tgt)
{
	struct stat tmp;
	int s, t, chr;
	char buf[BUFFERSIZE];

	
	if ((s = open(src, O_RDONLY)) == -1) 
	{
		erreur("Erreur lors de l'ouverture !", src);
	}

	if ((t = open(tgt, O_WRONLY | O_CREAT | O_TRUNC, tmp.st_mode)) == -1) 
	{
		erreur("Erreur lors de la création !", tgt);
	}

	
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

	
	if (close(s) == -1 || close(t) == -1)
	{
		printf("Erreur fermeture des fichiers !");
		exit(EXIT_FAILURE);
	}
}