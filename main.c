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
    GHashTable *ht = g_hash_table_new(g_str_hash, g_str_equal);
    while ((lineSize = getline(&linePtr, &n, f)) != -1) {
        char *nCoord = NULL;
        char *cSep = NULL;
        char *a = strtok_r(linePtr, ",", &cSep);
        char *b = strtok_r(NULL, ">", &cSep);
        int column = (int) strtol(a, NULL, 10);
        int row = (int) strtol(b, NULL, 10);
        if (row > yMax) {
            yMax = row;
        }
        while (*cSep != '\0') {
            a = strtok_r(NULL, ",", &cSep);
            b = strtok_r(NULL, ">", &cSep);

            int tCol = (int) strtol(a, NULL, 10);
            int tRow = (int) strtol(b, NULL, 10);

            if (tRow > yMax) {
                yMax = tRow;
            }
            int ca = column;
            int cb = tCol;
            int ra = row;
            int rb = tRow;

            if (tCol > column) {
                ca = tCol;
                cb = column;
            }
            if (tRow > row) {
                ra = tRow;
                rb = row;
            }
            if (ca != cb) {
                for (int i = cb; i <= ca; ++i) {
                    char *s = calloc(8, sizeof(char));
                    sprintf(s, "%d,%d", row, i);
                    g_hash_table_add(ht, s);
                }
            }
            if (ra != rb) {
                for (int i = rb; i <= ra; ++i) {
                    char *s = calloc(8, sizeof(char));
                    sprintf(s, "%d,%d", i, column);
                    g_hash_table_add(ht, s);
                }
            }

            column = tCol;
            row = tRow;
        }
    }
    bool run = true;
    while (run) {
        int row = 0;
        int col = 500;
        int run2 = true;
        while (run2) {
            char *s = calloc(8, sizeof(char));
            if (row > yMax) {
                run = false;
                run2 = false;
            }
            sprintf(s, "%d,%d", row+1, col);
            if (g_hash_table_contains(ht, s)) {
                sprintf(s, "%d,%d", row+1, col-1);
                if (g_hash_table_contains(ht, s)) {
                    sprintf(s, "%d,%d", row+1, col+1);
                    if (g_hash_table_contains(ht, s)) {
                        count++;
                        sprintf(s, "%d,%d", row, col);
                        g_hash_table_add(ht, s);
                        run2 = false;
                    }else {
                        row++;
                        col++;
                    }
                }else {
                    row++;
                    col--;
                }
            }else {
                row++;
            }
        }
    }
    printf("%d", count);

    return 0;
}
