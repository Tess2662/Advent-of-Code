#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

int main() {
    int win, lose, draw, score = 0;
    win = 6;
    lose = 0;
    draw = 3;
    int ascii[128] = {0};
    ascii['X'] = 1;
    ascii['Y'] = 2;
    ascii['Z'] = 3;


    FILE *f = fopen("/home/tereza/CLionProjects/aoc2022/input.txt", "r");
    char buff[256];
    while (fgets(buff, sizeof(buff), f)) {
        char enemy = buff[0];
        char me = buff[2];
        int diff = me-enemy-23;
        if (diff == 0) {
            printf("draw");
            score += draw;
        }
        else if (diff == 1 || diff == -2) {
            score += win;
        }
        else {
            score += lose;
        }
        printf("%d %d %c %c\n",diff,ascii[me],enemy,me);
        score += ascii[me];
    }
    printf("%d", score);
}
