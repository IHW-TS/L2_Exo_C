#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{

    int Louise_life;
    int Nb_fois_Teoman_la_fera_chier;
    int Nb_fois_Teoman_est_gentil_avec_louise;
    int Declaration_de_guerre;
    int Traite_de_paix;

    if (Louise_life > 0)
    {
        Nb_fois_Teoman_la_fera_chier += 1;
        Louise_life -= 1;
    }
    else if (Louise_life < 0)
    {
        Nb_fois_Teoman_est_gentil_avec_louise += 1;
        Louise_life += 1;
    }
    else
    {
        // Condition pour Louise_life = 0
        Traite_de_paix += 1;

        if (Traite_de_paix > 1)
        {
            Declaration_de_guerre;
            return EXIT_SUCCESS;
        }
        exit(EXIT_FAILURE);
    }
}
