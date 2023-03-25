#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <limits.h>

#define STACK_SIZE 128

void push(long *stack, int *top, long value) {
    if (*top >= STACK_SIZE) {
        exit(254);
    }
    stack[(*top)++] = value;
}

long pop(long *stack, int *top) {
    if (*top <= 0) {
        exit(253);
    }
    return stack[--(*top)];
}

long parse_number(const char *str, int base) {
    char *endptr;
    long value = strtol(str, &endptr, base);

    if (*endptr != '\0' || errno == ERANGE) {
        exit(251);
    }

    return value;
}

int main(int argc, char *argv[]) {
    long stack[STACK_SIZE];
    int top = 0;
    int base = 10;
    char *base_env = getenv("BASE");

    if (base_env != NULL) {
        int tmp_base = atoi(base_env);
        if (tmp_base >= 2 && tmp_base <= 16) {
            base = tmp_base;
        } else {
            exit(250);
        }
    }

    for (int i = 1; i < argc; i++) {
        if (strchr("+-*/", argv[i][0]) != NULL && argv[i][1] == '\0') {
            long b = pop(stack, &top);
            long a = pop(stack, &top);

            switch (argv[i][0]) {
                case '+':
                    push(stack, &top, a + b);
                    break;
                case '-':
                    push(stack, &top, a - b);
                    break;
                case '*':
                    push(stack, &top, a * b);
                    break;
                case '/':
                    if (b == 0) {
                        exit(252);
                    }
                    push(stack, &top, a / b);
                    break;
            }
        } else {
            long value = parse_number(argv[i], base);
            push(stack, &top, value);
        }
    }

    if (top == 0) {
        return EXIT_SUCCESS;
    }

    long result = pop(stack, &top);
    printf("%ld\n", result);

    if (top > 0) {
        exit(255);
    }

    return EXIT_SUCCESS;
}
