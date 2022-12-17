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
    unsigned long items[256];
    int itemCount;
    unsigned long test;
    int t;
    int f;
    unsigned long allTimeCount;
    unsigned long (*func)(unsigned long, unsigned long);
    unsigned long funcArg;
} monkey;

unsigned long add(unsigned long a, unsigned long b) {
    if (b == 0) {
        return a+a;
    }
    return a + b;
}

unsigned long mul(unsigned long a, unsigned long b) {
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
    unsigned long gcd = 1;
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
                m->items[m->itemCount] = (unsigned long) strtol(tokens[itok], NULL, 10);
                m->itemCount++;
                itok++;
            }
        }
        else if (strcmp(tokens[0],"Operation:") == 0) {
            unsigned  long arg = (unsigned long) strtol(tokens[5], NULL, 10);
            if (strcmp(tokens[4], "+") == 0) {
                monkeys[monkeyCount - 1].func = add;
            } else {
                monkeys[monkeyCount - 1].func = mul;
            }
            monkeys[monkeyCount - 1].funcArg = arg;
        }
        else if (strcmp(tokens[0], "Test:") == 0) {
           monkeys[monkeyCount - 1].test = (unsigned long) strtol(tokens[3], NULL, 10);
                gcd *= monkeys[monkeyCount - 1].test;
        }
        else if (tokens[1] && strcmp(tokens[1], "true:") == 0) {
            monkeys[monkeyCount - 1].t = (int) strtol(tokens[5], NULL, 10);
        }
        else if (tokens[1] && strcmp(tokens[1], "false:") == 0) {
           monkeys[monkeyCount - 1].f = (int) strtol(tokens[5], NULL, 10);
        }
        free(tokens);
    }
    for (int i = 0; i < 10000; ++i) {
        for (int j = 0; j < monkeyCount; ++j) {
            monkey* m = &monkeys[j];
            int max = m->itemCount;
            for (int k = 0; k < max; k++) {
                unsigned long temp = m->func(m->items[k], m->funcArg);
//                temp /= 3;
temp %= gcd;
                monkey *m2;
                if (temp % m->test == 0) {
                    m2 = &monkeys[m->t];
                }
                else {
                    m2 = &monkeys[m->f];
                }
                m2->items[m2->itemCount] = temp;
                m2->itemCount++;
                m->allTimeCount++;
                m->itemCount--;
            }
            if (i % 1000 == 999 || i == 19 || i == 0)
            printf("Monkey %d %d has %lu items\n", m->number,i, m->allTimeCount);
        }
    }
    ulong m1 = 0;
    ulong m2 = 0;
    for (int i = 0; i < monkeyCount; ++i) {
        if (monkeys[i].allTimeCount > m2) {
            if (monkeys[i].allTimeCount > m1) {
                m2 = m1;
                m1 = monkeys[i].allTimeCount;
            }
            else {
                m2 = monkeys[i].allTimeCount;
            }
        }
        printf("Monkey %d: %ld\n", i + 1, monkeys[i].allTimeCount);
    }
    printf("Max: %ld\n", m1);
    printf("Max2: %ld\n", m2);
    printf("%ld\n", m1*m2);


    free(monkeys);
    return 0;
}