#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>
#include <sys/types.h>
#include <malloc.h>
#include <stdlib.h>
#include <glib-2.0/glib.h>
#include <ctype.h>

int main() {
    FILE *f = fopen("/home/tereza/CLionProjects/aoc2022/input.txt", "r");
    char *linePtr = NULL;
    size_t n = 0;
    size_t lineSize;
    int sum = 1;
    int currentSum = 0;
    int nextPos = 0;

    while ((lineSize = getline(&linePtr, &n, f)) != -1) {
        linePtr[strcspn(linePtr, "\n\r")] = '\0';
        char *tok = strtok(linePtr, " ");
        int add = 0;
        if (strcmp(tok, "noop") == 0) {
            char c = nextPos % 40 <= sum + 1 && nextPos % 40 >= sum - 1 ? '#' : '.';
            printf("%c", c);
            nextPos ++;
        } else {
            add = (int) strtol(strtok(NULL, " "), NULL, 10);
            char c = nextPos % 40 <= sum + 1 && nextPos % 40 >= sum - 1 ? '#' : '.';
            printf("%c", c);
            nextPos++;
            if ((nextPos) % 40 == 0) {
                printf("\n");
            }
            c = nextPos % 40 <= sum + 1 && nextPos % 40 >= sum - 1 ? '#' : '.';
            printf("%c", c);
            nextPos++;
        }
        if ((nextPos) % 40 == 0) {
            printf("\n");
        }
        sum += add;
//        printf("%d\n",sum);


    }
    printf("%d", currentSum);
    return 0;
}