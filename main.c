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

char *linePtr = NULL;
size_t n = 0;
size_t lineSize;
int count = 0;
FILE *f;


typedef struct mFacing Facing;
typedef struct mConnection Connection;

struct mConnection {
    int dir;
    Facing *dst;
};
struct mFacing {
    int x;
    int y;
    // 0 - right, down, left, up
    Connection cons[4];
};

Facing facings[6];
int fCount = 0;
char **grid;
size_t columns = 0;
size_t rows = 0;
int cubeSize = 4;
char *moves;
int cCount = 0;

void allocateGrid() {
    grid = calloc(rows, sizeof(char *));
    for (int i = 0; i < rows; ++i) {
        grid[i] = calloc(columns + 1, sizeof(char));
    }

}

void countLines() {
    while ((lineSize = getline(&linePtr, &n, f)) != -1) {
        if (lineSize < 2) {
            break;
        }
        rows++;
        if (lineSize - 1 > columns) {
            columns = lineSize - 1;
        }
    }
}

void parseInput() {
    while ((lineSize = getline(&linePtr, &n, f)) != -1) {
        if (lineSize < 2) {
            break;
        }
        linePtr[strcspn(linePtr, "\n\r")] = 0;
        strncpy(grid[count], linePtr, columns);
        if (count % cubeSize == 0) {
            for (int i = 0; i < columns; i += cubeSize) {
                if (linePtr[i] == '.' || linePtr[i] == '#') {
                    facings[fCount] = (Facing) {i, count};
                    fCount++;
                }
            }
        }
        count++;

    }
    lineSize = getline(&linePtr, &n, f);
    moves = calloc(lineSize + 1, sizeof(char));
    strncpy(moves, linePtr, lineSize + 1);

}

void findSiblings() {
    for (int i = 0; i < fCount; ++i) {
        for (int j = i + 1; j < fCount; ++j) {
            if (facings[j].x - facings[i].x == cubeSize && facings[j].y == facings[i].y) {
                facings[i].cons[0] = (Connection) {0, &facings[j]};
                facings[j].cons[2] = (Connection) {2, &facings[i]};
                cCount += 2;
            } else if (facings[j].y - facings[i].y == cubeSize && facings[j].x == facings[i].x) {
                cCount += 2;
                facings[i].cons[1] = (Connection) {1, &facings[j]};
                facings[j].cons[3] = (Connection) {3, &facings[i]};
            }
        }
    }
}

bool containsFaceDst(Connection * c, Facing * f) {
    for (int i = 0; i < 4; ++i) {
        if (c[i].dst == f) {
            return true;
        }
    }
    return false;
}

void connectDisconnectedChilds(Facing *facing) {
    Facing *ff;
    Connection *cs = facing->cons;
    Facing * first,*second;
    int firstOut, secondOut;
    int firstIn, secondIn;
    for (int i = 0; i < 4; ++i) {
        int j = (i + 1) % 4;
        first = cs[i].dst;
        second = cs[j].dst;
        firstIn = (4 + cs[i].dir -1) % 4;
        secondIn = (cs[j].dir +1) % 4;
        firstOut = (firstIn + 2) % 4;
        secondOut = (secondIn + 2) % 4;
        if (first && second) {
            if (!first->cons[firstOut].dst) {
                assert(!second->cons[secondOut].dst);
                first->cons[firstOut] = (Connection) {secondIn, second};
                second->cons[secondOut] = (Connection) {firstIn, first};
                cCount+=2;
            }
        }

    }
}

void destroy() {
    free(moves);
    for (int i = 0; i < rows; ++i) {
        free(grid[i]);
    }
    free(grid);
    free(linePtr);
}

int main() {
    f = fopen("/home/tereza/CLionProjects/aoc2022/input.txt", "r");
    countLines();
    allocateGrid();
    rewind(f);
    parseInput();
    findSiblings();

    while (cCount < 24) {
        for (int i = 0; i < fCount; ++i) {
            connectDisconnectedChilds(&facings[i]);
        }
    }

    Facing *ff;
    Connection *c;
    for (int i = 0; i < fCount; ++i) {
        ff = &facings[i];
        c = facings[i].cons;
        for (int j = 0; j < 4; ++j) {
            assert(c[j].dst->cons[(c[j].dir + 2)%4].dst == ff);
            for (int k = j+1; k < 4; ++k) {
                assert(c[j].dst != c[k].dst);
            }
        }
    }


    destroy();
    return 0;
}