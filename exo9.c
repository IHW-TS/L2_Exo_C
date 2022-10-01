#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

// Creation d'un systeme de pile 

typedef struct ElementListe
{
    int nombre;
    struct ElementListe *suivant;
} Element;

// Struct qui continet l'adresse du premier elememnt de la pile 

typedef struct ListeRepere
{
    Element *debut;
} Pile;

// Fonction permettant d'initialiser une pile NULL

void initialisation(Pile *pile)
{
    pile->debut = NULL;
}

// Fonction afin d'empiler un élément à une pile
void empiler(Pile *pile, int nvNombre)
{
    //allocation dynamique pour une pile 
    Element *nouveau = malloc(sizeof(*nouveau)); 
    //ici le programme ca prendre en compte la taille de Element et retournera donc le bon nombre d'octet 
    if (pile == NULL || nouveau == NULL)
    {
        exit(EXIT_FAILURE); // sortit si la pile et vide 
    }

    nouveau->nombre = nvNombre;
    // entre un nombre en debut de parametre
    nouveau->suivant = pile->debut;
    // on va decaler le permier nombre et deuxieme position
    pile->debut = nouveau;
    // Va permettre a la prochaine valeur d'etre en debut de pile 

// Fonction pour dépiler un élément
int depiler(Pile *pile)
{
    if (pile == NULL)
    {
        exit(EXIT_FAILURE);
    }

    int nombreDepile = 0;
    Element *elementDepile = pile->debut;

    if (pile != NULL && pile->debut != NULL)
    {
        nombreDepile = elementDepile->nombre;
        // va permettre de separer le premier element du reste de la pile 
        pile->debut = elementDepile->suivant;

        //liberer un element 
        free(elementDepile);
    }

    return nombreDepile;
}

/* affichage de la pile */
void affichage(Pile *pile)
{
    if (pile == NULL)
    {
        exit(EXIT_FAILURE);
    }
    Element *actuel = pile->debut;

    while (actuel != NULL)
    {
        //affiche le premier 
        printf("%d\n", actuel->nombre);
        // passe au suivant 
        actuel = actuel->suivant;
    }

    printf("\n");
}

int main(int argc, const char *argv[])
{
    Pile pile;

    // Création de la pile
    initialisation(&pile);

    // variable nécessaire au passage d'un char à un int
    char *str;

    //boucle qui permet de verifier si sur l'entrée standard le charactere est un int ou un signe
     
    for (int i = 1; i < argc; i++)
    {

        // On passe le charactere dans l'entrée standard en simple char
        char c = (argv[i][0]);

        // si on a un int on empile le nombre
        // sinon on effectue l'opération entre les deux derniers nombres et on empile le tout
        if (c <= '9' && c >= '0')
        {
            //pour obtenir un entier à partir d'une chaîne on utilise strtol pour convertir une chaîne en int
            int nb = (int)strtol(argv[i], &str, 10);
            empiler(&pile, nb);
        }
        else if ((c == '/') || (c == '*') || (c == '-') || (c == '+'))
        {

            // on recupere le dernier élément
            int a = depiler(&pile);

            // on recupere le dernier element
            int b = depiler(&pile);

            // resultat de a 'operateur' b
            int res = 0;

            // on effectue les opérations correspondante
            if (c == '/')
            {
                if (a == 0)
                {
                    printf("252: Division par 0 !!!\n");
                    exit(-1);
                    break;
                }
            }
            else if (c == '*')
            {
                res = a * b;
            }
            else if (c == '+')
            {
                res = a + b;
            }
            else if (c == '-')
            {
                res = a - b;
            }

            empiler(&pile, res);
        }
        else
        {
            // Le char est ni un opérateur ni un nombre
            printf("251: Verifiez les entrées \n");
            exit(-1);
            break;
        }
    }

    affichage(&pile);

    printf("EXIT_SUCCESS\n");
    exit(0);
}
