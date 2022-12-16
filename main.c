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
    int lines = 1;
    int line = 0;
    while ((lineSize = getline(&linePtr, &n, f)) != -1) {
        line++;
        linePtr[strcspn(linePtr, "\n\r")] = '\0';
        char * tok = strtok(linePtr, " ");
        int add = 0;
        if (strcmp(tok,"noop") == 0) {
            lines+=1;
        }else {
            add = (int) strtol(strtok(NULL," "),NULL, 10);
            lines+=2;
        }if ((lines-20) % 40 == 0) {
           currentSum += (lines) * (sum + add);
            printf("%d\n",(lines) * (sum + add));
        }else if ((lines-20) % 40 == 1 && strcmp(tok,"addx") == 0) {
            currentSum += (lines-1) * sum;
            printf("%d\n",(lines-1) * (sum));
        }
        sum+= add;

//        printf("%d\n",sum);


    }
    printf("%d",currentSum);
    return 0;
}