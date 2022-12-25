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

typedef struct {
    int x;
    int y;
    int z;
    bool visited;
    bool lava;
} Node;

typedef struct {
    bool visited;
    bool lava;
} Cube;

int minX, minY, minZ, maxX, maxY, maxZ;

int main() {
    FILE *f = fopen("/home/tereza/CLionProjects/aoc2022/input.txt", "r");
    char *linePtr = NULL;
    size_t n = 0;
    size_t lineSize;
    int count = 0;
    Node *lines = NULL;
    Cube ***cubes = NULL;
    while ((lineSize = getline(&linePtr, &n, f)) != -1) {
        count++;
        lines = realloc(lines, count * sizeof(Node));
        int x = (int) strtol(strtok(linePtr, ","), NULL, 10);
        int y = (int) strtol(strtok(NULL, ","), NULL, 10);
        int z = (int) strtol(strtok(NULL, ","), NULL, 10);
        lines[count - 1] = (Node) {x, y, z, false, true};
        if (x < minX) minX = x;
        if (y < minY) minY = y;
        if (z < minZ) minZ = z;
        if (x > maxX) maxX = x;
        if (y > maxY) maxY = y;
        if (z > maxZ) maxZ = z;


    }
    minX--;
    minY--;
    minZ--;
    maxX++;
    maxY++;
    maxZ++;
    maxX -= minX;
    maxY -= minY;
    maxZ -= minZ;


    cubes = calloc((maxX + 1), sizeof(Cube **));
    for (int i = 0; i <= maxX; i++) {
        cubes[i] = calloc((maxY + 1), sizeof(Cube *));
        for (int j = 0; j <= maxY; j++) {
            cubes[i][j] = calloc((maxZ + 1), sizeof(Cube));
            for (int k = 0; k <= maxZ; k++) {
                cubes[i][j][k] = (Cube) {false, false};
            }
        }
    }

    for (int i = 0; i < count; ++i) {
        cubes[lines[i].x - minX][lines[i].y - minY][lines[i].z - minZ].lava = true;
    }

    free(lines);

    int sides = 0;
    int queueSize = 1;
    int *queue = calloc(queueSize, 3 * sizeof(int));
    int nb[6][3] = {{1,  0,  0},
                    {-1, 0,  0},
                    {0,  1,  0},
                    {0,  -1, 0},
                    {0,  0,  1},
                    {0,  0,  -1}};
    while (queueSize != 0) {
        queueSize--;
        int x = queue[queueSize * 3];
        int y = queue[queueSize * 3 + 1];
        int z = queue[queueSize * 3 + 2];
        cubes[x][y][z].visited = true;
        for (int i = 0; i < 6; ++i) {
            int newX = x + nb[i][0];
            int newY = y + nb[i][1];
            int newZ = z + nb[i][2];
            if (newX >= 0 && newX <= maxX && newY >= 0 && newY <= maxY && newZ >= 0 && newZ <= maxZ) {
                if (!cubes[newX][newY][newZ].visited) {
                    if (cubes[newX][newY][newZ].lava) {
                        sides++;
                    } else {
                        queueSize++;
                        queue = realloc(queue, queueSize * 3 * sizeof(int));
                        queue[queueSize * 3 - 3] = newX;
                        queue[queueSize * 3 - 2] = newY;
                        queue[queueSize * 3 - 1] = newZ;
                        cubes[newX][newY][newZ].visited = true;
                    }
                }
            }
        }

    }

    free(queue);

    // free cubes
    for (int i = 0; i <= maxX; i++) {
        for (int j = 0; j <= maxY; j++) {
            free(cubes[i][j]);
        }
        free(cubes[i]);
    }
    free(cubes);

    printf("%d", sides);

    free(linePtr);
    return 0;
}