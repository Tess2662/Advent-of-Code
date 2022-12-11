#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>
#include <sys/types.h>
#include <malloc.h>
#include <stdlib.h>

struct node {
    struct node *parent;
    int size;
    char name[256];
    struct node * sib;
    struct node * child;
};

int getFolderSize(struct node * n, char * parent);
int getFolderSizeWithoutSib(struct node * n);

int main() {
    FILE *f = fopen("/home/tereza/CLionProjects/aoc2022/input.txt", "r");
    char *linePtr = NULL, *firstToken = NULL, *secondToken = NULL, *thirdToken = NULL;
    size_t n = 0;
    size_t lineSize;
    struct node *nodes = NULL;
    int nodesCount = 0;
    struct node *currentNode = NULL;
    bool lsMode = false;
    int min = INT_MAX;
    int diff = 0;

    while ((lineSize = getline(&linePtr, &n, f)) != -1) {
        firstToken = strtok(linePtr, " ");
        secondToken = strtok(NULL, " ");
        thirdToken = strtok(NULL, " ");
        if (strcmp(firstToken, "$") == 0) {
            lsMode = false;
            if (strcmp(secondToken, "cd") == 0) {
                if (strcmp(thirdToken, "..\n") == 0) {
                    currentNode = currentNode->parent;
                } else {
                    nodesCount++;
                    nodes = (struct node *) realloc(nodes, sizeof(struct node) * nodesCount);
                    struct node * ptr = (nodes + nodesCount-1);
                    *ptr = (struct node){.parent = currentNode, .size = 0, .name = *thirdToken, .child = NULL, .sib = NULL};
                    if (currentNode != NULL) {
                        if (currentNode->child == NULL) {
                            currentNode->child = ptr;
                        } else {
                            struct node * s = currentNode->child;
                            while (strcmp(s->name, ptr->name) != 0 && s->sib != NULL) {
                                s = s->sib;
                            }
                            if (strcmp(s->name, ptr->name) == 0) {
                                printf("equals %s %s", ptr->name, s->name);
                                ptr = s;
                            }
                            else {

                                s->sib = ptr;
                            }
                        }
                    }
                    strcpy(ptr->name, thirdToken);
                    currentNode = ptr;
//                    printf("%s %s %s", firstToken, secondToken, thirdToken);
                }
            }
            else if (strcmp(secondToken, "ls\n") == 0) {
                lsMode = true;
            }
        }
        else if (strcmp(firstToken, "dir") != 0 && lsMode) {
            // file in directory
            currentNode->size +=(int) strtol(firstToken, NULL, 10);
//            printf("%d", currentNode->size);
        }
    }
    for (int i = 0; i < nodesCount; ++i) {
//        if ((nodes+i)->child) {
//            printf("%s ", (nodes+i)->child->name);
//            struct node * c = (nodes+i)->child;
//            struct node * s = c->sib;
//            while (s != NULL) {
//                printf("%s ", s->name);
//                s = s->sib;
//            }
//        }

        int sum = getFolderSizeWithoutSib(nodes+i);
        if (i == 0) {
            diff = 30000000-(70000000-sum);
            printf("%d\n", diff);
        }
        if (sum >= diff) {
            if (sum < min) {

                printf("\nname: %s",(nodes+i)->name);
                printf("%d %s", sum, (nodes+i)->name);
                min = sum;
            }
        }
    }
    printf("%d", min);

}
int getFolderSize(struct node * n, char * parent) {
    if (n == NULL) {
       return 0;
    }
//    printf("'''side parent of %s %s %d'''\n", n->name, parent, n->size);
    return getFolderSize(n->sib, parent) + getFolderSize(n->child, parent) + n->size;
}

int getFolderSizeWithoutSib(struct node * n) {
    if (n == NULL) {
        return 0;
    }
//    printf("main parent %s\n", n->name);
    return  getFolderSize(n->child, n->name) + n->size;
}
