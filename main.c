#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>
#include <sys/types.h>
#include <malloc.h>
#include <stdlib.h>
#include <glib-2.0/glib.h>
#include <ctype.h>

typedef struct Monkey {
    int number;
    int items[256];
    int itemCount;
    int test;
    int t;
    int f;
    int allTimeCount;
    int (*func)(int, int);
    int funcArg;
} monkey;

int add(int a, int b) {
    if (b == 0) {
        return a+a;
    }
    return a + b;
}

int mul(int a, int b) {
    if (b == 0) {
        return a*a;
    }
    return a * b;
}

int main() {
    FILE *f = fopen("/home/tereza/CLionProjects/aoc2022/input.txt", "r");
    char *linePtr = NULL;
    size_t n = 0;
    size_t lineSize;
    int monkeyCount = 0;
    monkey *monkeys = NULL;
    char **tokens;
    while ((lineSize = getline(&linePtr, &n, f)) != -1) {
        tokens = calloc(lineSize / 2 + 1, sizeof(char *));
        int c = 0;
        char *tok = strtok(linePtr, " ");
        tokens[c] = tok;
        c++;
        while ((tok = strtok(NULL, " "))) {
            tokens[c] = tok;
            c++;
        }
        if (strcmp(tokens[0], "Monkey") == 0) {
            monkeyCount++;
            monkeys = realloc(monkeys, sizeof(monkey) * monkeyCount);
            monkeys[monkeyCount -
                    1] = (monkey) {.allTimeCount = 0, .itemCount = 0, .number = monkeyCount, .test = 0, .t = 0, .f = 0};
        } else if (strcmp(tokens[0], "Starting") == 0) {
            int itok = 2;
            while (tokens[itok] != NULL) {
                monkey* m = &monkeys[monkeyCount - 1];
                m->items[m->itemCount] = (int) strtol(tokens[itok], NULL, 10);
                m->itemCount++;
                itok++;
            }
        }
        else if (strcmp(tokens[0],"Operation:") == 0) {
            if (strcmp(tokens[4], "+") == 0) {
                monkeys[monkeyCount - 1].func = add;
            } else {
                monkeys[monkeyCount - 1].func = mul;
            }
            monkeys[monkeyCount - 1].funcArg = (int) strtol(tokens[5], NULL, 10);
        }
        else if (strcmp(tokens[0], "Test:") == 0) {
           monkeys[monkeyCount - 1].test = (int) strtol(tokens[3], NULL, 10);
        }
        else if (tokens[1] && strcmp(tokens[1], "true:") == 0) {
            monkeys[monkeyCount - 1].t = (int) strtol(tokens[5], NULL, 10);
        }
        else if (tokens[1] && strcmp(tokens[1], "false:") == 0) {
           monkeys[monkeyCount - 1].f = (int) strtol(tokens[5], NULL, 10);
        }
        free(tokens);
    }
    for (int i = 0; i < 20; ++i) {
        for (int j = 0; j < monkeyCount; ++j) {
            monkey* m = &monkeys[j];
            for (int k = 0; k < m->itemCount;m->itemCount--,m->allTimeCount++) {
                int temp = m->func(m->items[m->itemCount-1], m->funcArg);
                temp /= 3;
                monkey *m2;
                if (temp % m->test == 0) {
                    m2 = &monkeys[m->t];
                }
                else {
                    m2 = &monkeys[m->f];
                }
                m2->items[m2->itemCount] = temp;
                m2->itemCount++;
            }
        }
    }
    for (int i = 0; i < monkeyCount; ++i) {
        printf("Monkey %d: %d\n", i + 1, monkeys[i].allTimeCount);
    }


    free(monkeys);
    return 0;
}