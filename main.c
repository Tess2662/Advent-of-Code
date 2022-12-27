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
    int value;
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
    int value;
    int prev;
    int next;
    FILE *f = fopen("/home/tereza/CLionProjects/aoc2022/input.txt", "r");
    while ((lineSize = getline(&linePtr, &n, f)) != -1) {
        value = (int) strtol(linePtr, NULL, 10);
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


    struct Node * x = zero;
    for (int i = 0; i < elementsCount; ++i) {
        printf("%d ", x->value)    ;
        x = x->next;
    }
    printf("\n");
    x = zero;
    for (int i = elementsCount; i > 0; --i) {
        printf("%d ", x->value)    ;
        x = x->prev;
    }
    printf("\n");

    struct Node * toBeMoved;
    struct Node * currentLocation;
    int tempValue;
    for (int i = 0; i < elementsCount; ++i) {
        toBeMoved = nodeQueue[i];
        currentLocation = toBeMoved;
        tempValue = toBeMoved->value;
        toBeMoved->value %= (elementsCount - 1);
        if (toBeMoved->value > 0) {
            for (int j = 0; j < toBeMoved->value; ++j) {
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
            ass:
            printf("%d\n",i);


        }
        else if (toBeMoved->value < 0) {
            for (int j = toBeMoved->value; j < 0 ; ++j) {
                currentLocation =  currentLocation->prev;
            }
            struct Node * desiredNext = currentLocation;
            struct Node * desiredPrev = currentLocation->prev;
            if (desiredNext == toBeMoved->next || currentLocation == toBeMoved) {
                printf("ass\n");
                goto ass2;
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
            ass2:
            printf("%d\n",i);

        }
        toBeMoved->value = tempValue;
        struct Node * x = zero;
        for (int i = 0; i < elementsCount; ++i) {
            printf("%d ", x->value)    ;
            x = x->next;
        }
        printf("\n");

   /*     x = zero;
        for (int i = elementsCount; i > 0; --i) {
            printf("%d ", x->value)    ;
            x = x->prev;
        }
        printf("\n");*/

    }
    int mix = 0;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 1000; ++j) {
            zero = zero->next;
        }
        mix+=zero->value;
        printf("%d\n",zero->value);
    }


    printf("%d",mix);
    free(linePtr);

    return 0;
}
