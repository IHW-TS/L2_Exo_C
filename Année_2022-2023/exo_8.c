#include <stdio.h>
#include <stdlib.h>

#define DEFAULT_WIDTH 7
#define DEFAULT_HEIGHT 6
#define DEFAULT_PLAYERS 2

int get_env_value(const char *env_var, int min, int max, int default_value) {
    const char *value_str = getenv(env_var);
    if (value_str == NULL) {
        return default_value;
    }

    int value = atoi(value_str);
    if (value < min || value > max) {
        return default_value;
    }

    return value;
}

int main(int argc, char *argv[]) {
    int width = DEFAULT_WIDTH;
    int height = DEFAULT_HEIGHT;
    int players = DEFAULT_PLAYERS;

    if (argc > 1) {
        int w = atoi(argv[1]);
        if (w >= 4 && w <= 26) {
            width = w;
        } else {
            width = get_env_value("P4WIDTH", 4, 26, DEFAULT_WIDTH);
        }
    }

    if (argc > 2) {
        int h = atoi(argv[2]);
        if (h >= 4 && h <= 16) {
            height = h;
        } else {
            height = get_env_value("P4HEIGHT", 4, 16, DEFAULT_HEIGHT);
        }
    }

    if (argc > 3) {
        int p = atoi(argv[3]);
        if (p >= 2 && p <= 8) {
            players = p;
        } else {
            players = get_env_value("P4PLAYERS", 2, 8, DEFAULT_PLAYERS);
        }
    }

    // Continue with your Puissance 4 game implementation here

    printf("Grid width: %d\n", width);
    printf("Grid height: %d\n", height);
    printf("Number of players: %d\n", players);

    return 0;
}
