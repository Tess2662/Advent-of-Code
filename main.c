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

struct Node{
    long value;
    struct Node *prev;
    struct Node *next;
};

char *linePtr = NULL;
size_t n = 0;
size_t lineSize;
int count = 0;
int const elementsCount = 5000;

struct Node nodes[5000];
struct Node * nodeQueue[5000];
struct Node *zero;


int main() {
    long value;
    int prev;
    int next;
    FILE *f = fopen("/home/tereza/CLionProjects/aoc2022/input.txt", "r");
    while ((lineSize = getline(&linePtr, &n, f)) != -1) {
        value = strtol(linePtr, NULL, 10)*811589153;
        next = count + 1;
        prev = count - 1;
        if (count == 0) {
            prev = elementsCount-1;
        }
        if (count == elementsCount-1) {
            next = 0;
        }
        nodes[count] = (struct Node) {value,  &nodes[prev],  &nodes[next]};
        nodeQueue[count] = &nodes[count];
        if (value == 0) {
            zero = &nodes[count];
        }
        count++;
    }



    struct Node * toBeMoved;
    struct Node * currentLocation;
    long tempValue;
    for (int l = 0; l < 10; ++l)
    {
        for (long i = 0; i < elementsCount; ++i) {
            toBeMoved = nodeQueue[i];
            currentLocation = toBeMoved;
            tempValue = toBeMoved->value;
            toBeMoved->value %= (elementsCount - 1);
            if (toBeMoved->value > 0) {
                for (long j = 0; j < toBeMoved->value; ++j) {
                    currentLocation =  currentLocation->next;
                }
                struct Node * desiredNext = currentLocation->next;
                struct Node * desiredPrev = currentLocation;
                // removing node

                if (desiredPrev == toBeMoved->prev || currentLocation == toBeMoved) {
                    printf("ass\n");
                    goto ass;
                }
                toBeMoved->prev->next = toBeMoved->next;
                toBeMoved->next->prev = toBeMoved->prev;

                // moving node
                toBeMoved->prev =  currentLocation;
                toBeMoved->next = currentLocation->next;

                toBeMoved->next->prev = toBeMoved;
                toBeMoved->prev->next = toBeMoved;

                assert(desiredNext != desiredPrev);
                assert(desiredPrev != toBeMoved);
                assert(toBeMoved->next == desiredNext);
                assert(toBeMoved->prev == desiredPrev);
                assert(desiredPrev->next == toBeMoved);
                assert(desiredNext->prev == toBeMoved);


            }
            else if (toBeMoved->value < 0) {
                for (long j = toBeMoved->value; j < 0 ; ++j) {
                    currentLocation =  currentLocation->prev;
                }
                struct Node * desiredNext = currentLocation;
                struct Node * desiredPrev = currentLocation->prev;
                if (desiredNext == toBeMoved->next || currentLocation == toBeMoved) {
                    printf("ass\n");
                    goto ass;
                }
                // removing node
                toBeMoved->prev->next = toBeMoved->next;
                toBeMoved->next->prev = toBeMoved->prev;

                // moving node
                toBeMoved->next =  currentLocation;
                toBeMoved->prev = currentLocation->prev;

                toBeMoved->next->prev = toBeMoved;
                toBeMoved->prev->next = toBeMoved;

                assert(desiredNext != desiredPrev);
                assert(desiredPrev != toBeMoved);
                assert(toBeMoved->next == desiredNext);
                assert(toBeMoved->prev == desiredPrev);
                assert(desiredPrev->next == toBeMoved);
                assert(desiredNext->prev == toBeMoved);

            }
            ass:
            toBeMoved->value = tempValue;


            /*     x = zero;
                 for (int i = elementsCount; i > 0; --i) {
                     printf("%d ", x->value)    ;
                     x = x->prev;
                 }
                 printf("\n");*/

        }
    }
    long mix = 0;
    for (long i = 0; i < 3; ++i) {
        for (long j = 0; j < 1000; ++j) {
            zero = zero->next;
        }
        mix+=zero->value;
        printf("%ld\n",zero->value);
    }


    printf("%ld",mix);
    free(linePtr);

    return 0;
}
