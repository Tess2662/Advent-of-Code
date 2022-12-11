#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

int main() {
    int win, lose, draw, score = 0;
    win = 6;
    lose = 0;
    draw = 3;
    int ascii[128] = {0};
    ascii['X'] = 0;
    ascii['Y'] = 3;
    ascii['Z'] = 6;
    ascii['A'] = 1;
    ascii['B'] = 2;
    ascii['C'] = 3;
    ascii['D'] = 1;
    ascii[64] = 3;


    FILE *f = fopen("/home/tereza/CLionProjects/aoc2022/input.txt", "r");
    char buff[256];
    while (fgets(buff, sizeof(buff), f)) {
        char enemy = buff[0];
        int move;
        char me = buff[2];
        int diff = me-enemy-23;
        if (me == 'X') {
           move =  enemy-1;
        }
        else if (me == 'Y') {
            move = enemy;
        }
        else {
            move = enemy +1;
        }
        score += ascii[move];
        score += ascii[me];
    }
    printf("%d", score);
}
