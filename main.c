#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>
#include <sys/types.h>
#include <malloc.h>
#include <stdlib.h>
#include <glib-2.0/glib.h>

void wtf(gpointer p, gpointer p1, gpointer p3) {
    printf("%s %s %s\n", (char *) p, (char *) p1, (char *) p3);
}

gboolean pred(gpointer a, gpointer b, gpointer c) {
    printf("%s %s %s\n", a, b, c);
    return (strcmp(a, b) == 0);
}

int main() {
    FILE *f = fopen("/home/tereza/CLionProjects/aoc2022/input.txt", "r");
    char *linePtr = NULL;
    size_t n = 0;
    size_t lineSize;
    GHashTable *ht = g_hash_table_new(g_str_hash, g_str_equal);
    int step = 0;
    char *dir;
    int nodes[10][2] = {0};
    while ((lineSize = getline(&linePtr, &n, f)) != -1) {
        dir = strtok(linePtr, " ");
        step = (int) strtol(strtok(NULL, " "), NULL, 10);
        int sign = (*dir - 'O');
        sign = sign / abs(sign);
        int a = 0;
        int b = 1;
        int w = 1;
        if (strspn(dir, "RL")) {
            w = 0;
        }
        for (int i = 0; i < step; ++i) {
            nodes[0][w] += sign;
            for (int j = 1; j < 10; ++j) {
                int diffA = nodes[j - 1][a] - nodes[j][a];
                int diffB = nodes[j - 1][b] - nodes[j][b];
                int diff = abs(diffA) + abs(diffB);
                if (diff < 2) {
                    break;
                }
                int stepA = 0;
                int stepB = 0;
                // diagonal
                if (diff > 2) {
                    stepA = diffA / abs(diffA);
                    stepB = diffB / abs(diffB);
                } else {
                    if (abs(diffA) > 1) {
                        stepA = diffA / abs(diffA);
                    } else if (abs(diffB) > 1) {
                        stepB = diffB / abs(diffB);
                    } else {
                        break;
                    }
                }
                nodes[j][a] += stepA;
                nodes[j][b] += stepB;
                if (j == 9) {
                    char *s = calloc(1, 20);
                    sprintf(s, "%d,%d", nodes[j][a], nodes[j][b]);
                        printf("%s\n", s);
                    g_hash_table_add(ht, s);
                }
            }
        }


//        for (int i = 0; i < 9; ++i) {
//            nodes[i][a] += sign * step;
//            int diffA= abs(nodes[i][a] - nodes[i + 1][a]) - 1;
//            int diffB = abs(nodes[i][b] - nodes[i + 1][b]) - 1;
//            if (diffA + diffB > 1 ) {
//                // diagonal movement
//            }
//            if (diff > 0) {
//                nodes[i + 1][b] = nodes[i][b];
//            }
//            for (int j = 0; diff > j; ++j) {
//                nodes[i + 1][a] += sign;
//                char *s = calloc(1, 20);
//                sprintf(s, "%d,%d", nodes[i+1][a], nodes[i+1][b]);
//                g_hash_table_add(ht, s);
//            }
//            step = 0;
//        }
        unsigned int xx = g_hash_table_size(ht);
//        printf("%d", xx);
    }
//    g_hash_table_foreach(ht, wtf, NULL);
    gboolean what = g_hash_table_contains(ht, "0,0");
    printf("\n%d interesting %d what", g_hash_table_size(ht), what);
    g_hash_table_destroy(ht);
    return 0;
}