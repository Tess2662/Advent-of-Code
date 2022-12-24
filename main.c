#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>
#include <sys/types.h>
#include <malloc.h>
#include <stdlib.h>
#include <glib-2.0/glib.h>
#include <ctype.h>
#include <assert.h>

int main() {
    FILE *f = fopen("/home/tereza/CLionProjects/aoc2022/input.txt", "r");
    char *linePtr = NULL;
    size_t n = 0;
    size_t lineSize;
    int count = 0;
    int yMax = 0;
    int xB, yB, xS, yS;
    char * equalsPtr = NULL;
    int *  remove = NULL;
    int removeC = 0;
    int line = 2000000;
    GHashTable *ht = g_hash_table_new(g_str_hash, g_str_equal);
    while ((lineSize = getline(&linePtr, &n, f)) != -1) {
        strtok(linePtr, "=");
        xS = (int) strtol(strtok(NULL, "="), NULL, 10);
        yS = (int) strtol(strtok(NULL, "="), NULL, 10);
        xB = (int) strtol(strtok(NULL, "="), NULL, 10);
        yB = (int) strtol(strtok(NULL, "="), NULL, 10);
        int d = abs(xS - xB) + abs(yS - yB);
        int yD = d - abs(yS - line);
//        if (yS == line) {
//            removeC++;
//            remove = realloc(remove, removeC * sizeof(int));
//            remove[removeC - 1] =  xS;
//        }
        if (yB == line) {
            removeC++;
            remove = realloc(remove, removeC * sizeof(int));
            remove[removeC - 1] =  xB;
        }
        if (yD >= 0) {
            for (int i = xS -  yD; i <= xS + yD; ++i) {
                char *s = malloc(16);
                sprintf(s, "%d", i);
                g_hash_table_add(ht, s);
            }
        }
    }
    for (int i = 0; i < removeC; ++i) {
        char *s = malloc(16);
        sprintf(s, "%d", remove[i]);
        g_hash_table_remove(ht, s);

    }
    for (int i = 0; i < 40; ++i) {
        char * s = calloc(16, sizeof(char));
        sprintf(s,"%d", i);
        printf("%c", g_hash_table_contains(ht, s) ? '#' : '.');
    }
    printf("%d", g_hash_table_size(ht));
    g_hash_table_destroy(ht);
    free(linePtr);

    return 0;
}
