#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

int main() {
    FILE *f = fopen("/home/tereza/CLionProjects/aoc2022/input.txt", "r");
    char buff[256];
    int current = 0;
    int m[3];
    while (fgets(buff, sizeof(buff), f)) {
        if (isdigit(buff[0])) {
            current += atoi(buff);
        } else {
            if (current > m[2])
            {
                if (current > m[1]) {
                    if (current > m[0]) {
                        m[2] = m[1];
                        m[1] = m[0];
                        m[0] = current;
                    } else {
                        m[2] = m[1];
                        m[1] = current;
                    }
                } else {
                    m[2] = current;
                }
            }
            current = 0;
        }
    }
    if (current > m[2])
        m[2] = current;
    else if (current > m[1])
        m[1] = current;
    else if (current > m[0])
        m[0] = current;

    // print sum of array m
    printf("%d", m[0] + m[1] + m[2]);
}
