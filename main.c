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

typedef struct mNode Node;

struct mNode {
    long value;
    bool computed;
    char left[5];
    char right[5];
    char op;
};

Node *root;

char *linePtr = NULL;
size_t n = 0;
size_t lineSize;
int count = 0;

GHashTable *ht;


long get_value(Node *x) {
    assert(x != NULL);
    if (x->computed) {
        return x->value;
    }
    long v1 = get_value(g_hash_table_lookup(ht, x->left));
    long v2 = get_value(g_hash_table_lookup(ht, x->right));
    switch (x->op) {
        case '+':
            x->value = v1 + v2;
            break;
        case '-':
            x->value = v1 - v2;
            break;
        case '*':
            x->value = v1 * v2;
            break;
        case '/':
            x->value = v1 / v2;
            break;
    }
    printf("%ld %c %ld = %ld\n",v1,x->op,v2,x->value);
    x->computed = true;
    return x->value;


}

int main() {
    ht = g_hash_table_new(g_str_hash, g_str_equal);
    FILE *f = fopen("/home/tereza/CLionProjects/aoc2022/input.txt", "r");
    while ((lineSize = getline(&linePtr, &n, f)) != -1) {
        linePtr[strcspn(linePtr, "\r\n")] = '\0';
        char *name = calloc(sizeof(char), 5);
        strcpy(name, strtok(linePtr, ":"));
        Node *s = calloc(sizeof(Node), 1);
        if (isdigit(linePtr[6])) {
            s->value = (long) strtol(linePtr + 6, NULL, 10);
            s->computed = true;
        } else {
            strtok(NULL, " ");
            strtok(NULL, " ");
            strcpy(s->left, linePtr + 6);
            s->op = linePtr[11];
            strcpy(s->right, linePtr + 13);
            s->computed = false;
        }
        g_hash_table_insert(ht, name, s);
        if (strcmp(name, "root") == 0) {
            root = s;
        }

    }


    printf("--- %ld ---", get_value(root));
    free(linePtr);
    g_hash_table_destroy(ht);

    return 0;
}
