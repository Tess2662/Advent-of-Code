#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>
#include <sys/types.h>
#include <malloc.h>
#include <stdlib.h>
#include <glib-2.0/glib.h>
#include <ctype.h>

int getDst(const int dirs[2], int x, int y, const char * arr, int w, int h) {
    char cur = arr[y*w+x];
   int sum = 0;
    while (0 < x && x < w-1 && 0 < y && y < h-1) {
        x+=dirs[0];
        y+=dirs[1];
        char m = arr[y*w+x];
        if (cur > arr[y*w+x]) {
            sum++;
        }else {
            sum++;
            break;
        }
    }
    return sum;
}

int main() {
    FILE *f = fopen("/home/tereza/CLionProjects/aoc2022/input.txt", "r");
    char *linePtr = NULL;
    size_t n = 0;
    size_t lineSize;
    char * dyn = NULL;
    int size = 0;
    int l = 0;
    int max = 0;
    int xy[] = {0,0};
    int dirs[4][2] = {{-1,0},{1,0},{0,-1},{0,1}};
    while ((lineSize = getline(&linePtr, &n, f)) != -1) {
        linePtr[strcspn(linePtr, "\r\n")] = '\0';
        size++;
        l = (int)strlen(linePtr)+1;
        dyn = realloc(dyn, l * size);
        strcpy(dyn+l*(size-1),linePtr);
    }
    for (int i = 1; i < size - 1; ++i) {
        printf("%s\n",&dyn[l*i]);
        for (int j = 1; j < l - 1; ++j) {
            int res = 1;
            for (int k = 0; k < 4; ++k) {
               res *= getDst(dirs[k], j, i, dyn, l, size);
               if (res == 0){
                   break;
               }
            }
            if (res > max) {
                max = res;
                xy[0] = j;
                xy[1] = i;
            }
        }
    }
    printf("max %d %d %d\n", max, xy[0], xy[1]);

    return 0;
}