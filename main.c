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
    int * lines = NULL;
    GHashTable *ht = g_hash_table_new(g_str_hash, g_str_equal);
    while ((lineSize = getline(&linePtr, &n, f)) != -1) {
        count++;
        lines = realloc(lines, count * sizeof(int) * 3);
        int x = (int) strtol(strtok(linePtr, ","), NULL, 10);
        int y = (int) strtol(strtok(NULL, ","), NULL, 10);
        int z = (int) strtol(strtok(NULL, ","), NULL, 10);
        int t[3] = {x, y, z};
        memcpy(lines + (count - 1) * 3,t , sizeof(int) * 3);
}
    int sides = 0;
    for (int i = 0; i < count; ++i) {
        sides += 6;
        for (int j = 0; j < count; ++j) {
            int sum = 0;
            for (int k = 0; k < 3; ++k) {
                sum += abs(lines[i*3+k] - lines[j*3+k]);
            }
            if (sum == 1) {
sides--;
            }
        }
    }
    printf("%d", sides);
    free(lines);
    return 0;
}
