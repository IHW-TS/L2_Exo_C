#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// Définition du type number
typedef struct {
    unsigned char base;
    unsigned int length;
    char* digits;
} number;

// Fonction pour libérer la mémoire d'un number
void free_number(number nbr) {
    if (nbr.digits != NULL) {
        free(nbr.digits);
    }
}

// Fonction pour convertir un entier C en nombre en base quelconque
number to_number(unsigned int nbr, unsigned char base) {
    if (base != 2 && base != 4 && base != 8 && base != 16 && base != 32 && base != 64) {
        base = 16;
    }

    char symbols[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz+/";
    unsigned int length = log(nbr) / log(base) + 1;

    number result;
    result.base = base;
    result.length = length;
    result.digits = (char*)malloc(length * sizeof(char));

    for (int i = length - 1; i >= 0; i--) {
        result.digits[i] = symbols[nbr % base];
        nbr /= base;
    }

    return result;
}

// Fonction pour convertir un nombre en base quelconque en entier C
unsigned int to_uint(number nbr) {
    unsigned int result = 0;
    for (unsigned int i = 0; i < nbr.length; i++) {
        char digit = nbr.digits[i];
        unsigned int value;

        if (digit >= '0' && digit <= '9') {
            value = digit - '0';
        } else if (digit >= 'A' && digit <= 'Z') {
            value = digit - 'A' + 10;
        } else if (digit >= 'a' && digit <= 'z') {
            value = digit - 'a' + 36;
        } else {
            value = digit == '+' ? 62 : 63;
        }

        result += value * pow(nbr.base, nbr.length - 1 - i);
    }

    return result;
}

// Fonction pour convertir un nombre en base quelconque en chaîne de caractères
char* to_string(number nbr) {
    char* str = (char*)malloc((nbr.length + 1) * sizeof(char));
    strncpy(str, nbr.digits, nbr.length);
    str[nbr.length] = '\0';
    return str;
}

int main() {
    unsigned int n = 12345;
    unsigned char base = 16;

    number nbr = to_number(n, base);
    printf("Nombre en base %d: %s\n", base, nbr.digits);

    unsigned int c_uint = to_uint(nbr);
    printf("Nombre en entier C: %u\n", c_uint);

    char* str = to_string(nbr);
    printf("Nombre en chaîne de caractères: %s\n", str);

    free_number(nbr);
    free(str);

    return 0;
}
