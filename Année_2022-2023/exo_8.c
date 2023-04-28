// Inclusion de bibliothèques nécessaires.
#include <stdio.h>
#include <stdlib.h>

// Définition de constantes par défaut pour la largeur, la hauteur et le nombre de joueurs.
#define DEFAULT_WIDTH 7
#define DEFAULT_HEIGHT 6
#define DEFAULT_PLAYERS 2

// Fonction pour obtenir une valeur d'environnement, la valeur par défaut est renvoyée si la valeur de l'environnement n'est pas valide.
int get_env_value(const char *env_var, int min, int max, int default_value) {
    // Obtenir la valeur de l'environnement spécifié.
    const char *value_str = getenv(env_var);
    // Vérifier si la valeur n'est pas nulle, sinon renvoyer la valeur par défaut.
    if (value_str == NULL) {
        return default_value;
    }

    // Convertir la valeur de la chaîne en un entier.
    int value = atoi(value_str);
    // Vérifier si la valeur est comprise entre les limites spécifiées, sinon renvoyer la valeur par défaut.
    if (value < min || value > max) {
        return default_value;
    }

    // Renvoyer la valeur convertie.
    return value;
}

// Fonction principale du programme.
int main(int argc, char *argv[]) {
    // Définition des variables pour la largeur, la hauteur et le nombre de joueurs.
    int width = DEFAULT_WIDTH;
    int height = DEFAULT_HEIGHT;
    int players = DEFAULT_PLAYERS;

    // Vérifier si des arguments de ligne de commande ont été fournis pour la largeur.
    if (argc > 1) {
        int w = atoi(argv[1]);
        // Si la valeur est dans la plage valide, utiliser cette valeur pour la largeur.
        if (w >= 4 && w <= 26) {
            width = w;
        } else {
            // Sinon, obtenir la valeur de l'environnement.
            width = get_env_value("P4WIDTH", 4, 26, DEFAULT_WIDTH);
        }
    }

    // Vérifier si des arguments de ligne de commande ont été fournis pour la hauteur.
    if (argc > 2) {
        int h = atoi(argv[2]);
        // Si la valeur est dans la plage valide, utiliser cette valeur pour la hauteur.
        if (h >= 4 && h <= 16) {
            height = h;
        } else {
            // Sinon, obtenir la valeur de l'environnement.
            height = get_env_value("P4HEIGHT", 4, 16, DEFAULT_HEIGHT);
        }
    }

    // Vérifier si des arguments de ligne de commande ont été fournis pour le nombre de joueurs.
    if (argc > 3) {
        int p = atoi(argv[3]);
        // Si la valeur est dans la plage valide, utiliser cette valeur pour le nombre de joueurs.
        if (p >= 2 && p <= 8) {
            players = p;
        } else {
            // Sinon, obtenir la valeur de l'environnement.
            players = get_env_value("P4PLAYERS", 2, 8, DEFAULT_PLAYERS);
        }
    }

    // Continuer avec l'implémentation du jeu Puissance 4 ici.

    // Afficher les valeurs de la largeur, de la hauteur et du nombre de joueurs.
    printf("Grid width: %d\n", width
    printf("Grid height: %d\n", height);
    printf("Number of players: %d\n", players);

// Terminer le programme avec une valeur de sortie réussie.
return 0;
}
