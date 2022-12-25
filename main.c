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
#include <math.h>

typedef struct {
    int resources[4];
    int curRes;
    int neededMaterial[4][3];
    int bots[4];
} Blueprint;

char *linePtr = NULL;
size_t n = 0;
size_t lineSize;
int count = 0;
//Blueprint bp[30];

char *find_space(int c) {
    int s = 0;
    int ptr = 0;
    while (linePtr[ptr] != '\0' && s < c) {
        if (linePtr[ptr] == ' ') {
            s++;
        }
        ptr++;
    }
    return linePtr + ptr;
}


int main() {
    FILE *f = fopen("/home/tereza/CLionProjects/aoc2022/input.txt", "r");
    int sum = 0;
    while ((lineSize = getline(&linePtr, &n, f)) != -1) {
        count++;
        Blueprint bp = {{0, 0, 0, 0}, 0, {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}}, {1, 0, 0, 0}};
        int c;
        c = (int) strtol(find_space(6), NULL, 10);
        bp.neededMaterial[0][0] = c;
        c = (int) strtol(find_space(12), NULL, 10);
        bp.neededMaterial[1][0] = c;
        c = (int) strtol(find_space(18), NULL, 10);
        bp.neededMaterial[2][0] = c;
        c = (int) strtol(find_space(21), NULL, 10);
        bp.neededMaterial[2][1] = c;
        c = (int) strtol(find_space(27), NULL, 10);
        bp.neededMaterial[3][0] = c;
        c = (int) strtol(find_space(30), NULL, 10);
        bp.neededMaterial[3][2] = c;


        for (int i = 0; i < 24; ++i) {
            int rounds = 0;
            int roundResource = -1;
            if (bp.curRes < 3) {
                bool good = true;
                for (int j = 0; j < 3; ++j) {
                    if (bp.resources[j] < bp.neededMaterial[bp.curRes + 1][j]) {
                        good = false;
                        int diff = (int) (
                                (double) (bp.neededMaterial[bp.curRes + 1][j] - bp.resources[j]) / (double) bp.bots[j] +
                                1.0);
                        if (diff > rounds) {
                            rounds = diff;
                            roundResource = j;
                        }
                    }
                }
                if (good) {
                    bp.curRes++;
                    for (int k = 0; k < 3; ++k) {
                        bp.resources[k] -= bp.neededMaterial[bp.curRes][k];
                    }
                    // will be mistakenly added for this robot the next step
                    bp.resources[bp.curRes]--;
                    bp.bots[bp.curRes]++;
                }
            }
            for (int p = 0; p <= bp.curRes; ++p) {
                bool good = true;
                for (int j = 0; j < 3; ++j) {
                    if (bp.resources[j] < bp.neededMaterial[p][j]) {
                        good = false;
                    }
                }
                if (good) {
                    bp.bots[p]++;
                    int j = roundResource;
                    if (bp.bots[j] < 1) {
                        continue;
                    }
                    int tr = bp.resources[j] - bp.neededMaterial[p][j];
                    int diff = (int) (
                            (double) (bp.neededMaterial[bp.curRes + 1][j] - tr + 1) / (double) bp.bots[j] +
                            1.0);
                    // will make it faster
                    if (abs(diff) < rounds) {
                        for (int k = 0; k < 3; ++k) {
                            bp.resources[k] -= bp.neededMaterial[p][k];
                        }
                        // will be mistakenly added for this robot the next step
                        bp.resources[p]--;
                        goto if_end;
                    }
                    bp.bots[p]--;
                }

            }
            if_end:
            // ore za predchozi kolo
            for (int j = 0; j < 4; ++j) {
                bp.resources[j] += bp.bots[j];
            }

        }

        sum += bp.resources[3] * count;

    }

    printf("%d", sum);
    free(linePtr);

    return 0;
}
