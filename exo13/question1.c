void cp_mode(char *src, char *tgt)
{
    struct stat tmp;
    if (stat(src, &tmp) != 0) 
    {
        printf("Erreur Stat");
        exit(-1);
    }
    
    if (chmod(tgt, tmp.st_mode) != 0) /
    {
        printf("Erreur Stat");
        exit(-1);
    }
}