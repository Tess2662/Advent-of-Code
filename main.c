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
        count++;
        xS = (int) strtol(strtok(NULL, "="), NULL, 10);
        yS = (int) strtol(strtok(NULL, "="), NULL, 10);
        xB = (int) strtol(strtok(NULL, "="), NULL, 10);
        yB = (int) strtol(strtok(NULL, "="), NULL, 10);
        int d = abs(xS - xB) + abs(yS - yB);
        for (int i = -d; i <=d; ++i) {
            for (int j = xS - (d-abs(i))/2; j <= xS + (d-abs(i))/2; ++j) {
                char *s = malloc(16);
                sprintf(s, "%d,%d", yS+i, j);
                g_hash_table_add(ht, s);
            }
        }
//            for (int i = xS -  yD; i <= xS + yD; ++i) {
//                char *s = malloc(16);
//                sprintf(s, "%d", i);
//                g_hash_table_add(ht, s);
//        }
    }
    for (int i = 0; i < removeC; ++i) {
        char *s = malloc(16);
        sprintf(s, "%d", remove[i]);
        g_hash_table_remove(ht, s);

    }
    for (int i = 0; i < 20; ++i) {
        for (int j = 0; j < 20; ++j) {
            char * s = calloc(16, sizeof(char));
            sprintf(s,"%d,%d", i,j);
            printf("%c", g_hash_table_contains(ht, s) ? '#' : '.');
        }
        printf("\n");
    }
    printf("%d", g_hash_table_size(ht));
    g_hash_table_destroy(ht);
    free(linePtr);

    return 0;
}
