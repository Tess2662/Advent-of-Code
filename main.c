#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>
#include <sys/types.h>
#include <malloc.h>
#include <stdlib.h>
#include <glib-2.0/glib.h>
#include <ctype.h>
char * getPointer(int chars,int pos,int* count, char * arr) {
    return (arr +(pos * chars) + count[pos]);
}
int main() {
    FILE *f = fopen("/home/tereza/CLionProjects/aoc2022/input.txt", "r");
    char *linePtr = NULL;
    size_t n = 0;
    size_t lineSize;
    char *arr;
    int *count;
    long offsets[50] = {0};
    int line = 0;
    gboolean jumped = false;
    int chars = 0;
    int columns = 0;
    while ((lineSize = getline(&linePtr, &n, f)) != -1) {
        if (jumped == false) {
            line++;
            offsets[line] = ftell(f);
            gboolean space = false;
            char * tmpPtr = linePtr;
            while (*linePtr) {
                if (*linePtr == '[') {
                    chars++;
                    space = true;
                }
                linePtr++;
            }
            if (space == false) {
                jumped = true;
                char * tt = strtok(tmpPtr, " ");
                while (tt) {
                    tt = strtok(NULL," ");
                    columns++;
                }
                count = (int *) calloc(columns, sizeof(int));
                arr = (char *) calloc(columns, sizeof(char) * chars);
                tmpPtr = NULL;
                size_t xx = 0;
                for (int i = line - 2; i >= 0; --i) {
                    fseek(f, offsets[i], SEEK_SET);
                    getline(&tmpPtr, &xx, f);
//                    printf("wow %s", tmpPtr);
//                    char *st = strtok(tmpPtr, "[");
                    char *tok = strtok(tmpPtr, " ");
//                    char *tok = strsep(&tmpPtr,"[");
                    while (tok != NULL) {
                        int pos = (tok - tmpPtr) / 4;
                        arr[pos * chars + count[pos]] = tok[1];
//                        printf("%c\n", *getPointer(chars,pos,count,arr));
                    char * tmp = &arr[chars*pos];
                        count[pos] += 1;

//                        st = strtok(NULL, "[");
                        tok = strtok(NULL, " ");
                    }
                }
                fseek(f, offsets[line], SEEK_SET);

                getline(&tmpPtr, &xx, f);

            }
        } else {
            char * ptr;
            // cause seg fault idk why
//            printf("%s", linePtr);
//            strtok_r(linePtr," ", &ptr);
//            int howMany = atoi(strtok_r(NULL," ",&ptr));
//            strtok_r(NULL," ",&ptr);
//            int from =  atoi(strtok_r(NULL," ",&ptr));
//            strtok_r(NULL," ",&ptr);
//            int to =  atoi(strtok_r(NULL," ",&ptr));
            strsep(&linePtr, " ");
            int howMany =  atoi(strsep(&linePtr, " "));
            strsep(&linePtr, " ");
            int from =  atoi(strsep(&linePtr, " ")) -1;
            strsep(&linePtr, " ");
            int to =  atoi(strsep(&linePtr, " ")) -1;
            count[from]-=howMany;
            for (int i = 0; i < howMany; ++i) {
                char block = arr[from*chars + count[from]+i];
               arr[to*chars + count[to]+i] = block;
            }
            count[to]+=howMany;
        }
    }
    for (int i = 0; i < columns; ++i) {
        printf("%c",arr[chars*i + count[i]-1]);
    }
    return 0;
}