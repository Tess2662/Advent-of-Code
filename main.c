#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
int main()   {
    FILE* f = fopen("/home/tereza/CLionProjects/aoc2022/input.txt", "r");
    char buff[256];
    int max = 0;
    int current = 0;
    while (fgets(buff, sizeof(buff), f)) {
      if (isdigit(buff[0])) {
        current+= atoi(buff);
      } else {
        if (current > max) {
          max = current;
        }
        current = 0;
      }
    }
    max = current > max ? current : max;
    printf("%d", max);
}
