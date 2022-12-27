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
    char *name;
    bool lHuman;
    bool rHuman;
    char op;
    long rOrL;
};


Node *root;

char *linePtr = NULL;
size_t n = 0;
size_t lineSize;
int count = 0;

GHashTable *ht;

Node *human;


long get_value(Node *x) {
    assert(x != NULL);
    if (x->computed) {
        printf("%s %ld\n", x->name, x->value);
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
    printf("%s %ld %c %ld = %ld\n", x->name, v1, x->op, v2, x->value);
    x->computed = true;
    return x->value;
}

bool is_human_column(Node *x) {
    assert(x != NULL);
    if (strcmp(x->name, "humn") == 0) {
        x->lHuman = true;
        x->rHuman = true;
        return true;
    }
    if (x->computed) {
        return false;
    }

    bool h1 = is_human_column(g_hash_table_lookup(ht, x->left));
    bool h2 = is_human_column(g_hash_table_lookup(ht, x->right));
    assert(!(h1 && h2));
    if (h1) {
        x->lHuman = true;
    } else if (h2) {
        x->rHuman = true;
    }
    return (h1 || h2);

}

long backtrace_human(Node *x) {
    assert(x != NULL);
    assert(x->lHuman || x->rHuman);
    assert(x->computed);
    // human with value from previous krok
    if (x->lHuman && x->rHuman) {
        return x->value;
    }
    Node *h;
    long v;
    if (x->lHuman) {
       h =  g_hash_table_lookup(ht, x->left);
        v = get_value(g_hash_table_lookup(ht, x->right));
        switch (x->op) {
            case '-':
                x->rOrL = x->value + v;
                break;
            case '/':
                x->rOrL = x->value * v;
                break;
        }
    } else if (x->rHuman) {
        v = get_value(g_hash_table_lookup(ht, x->left));
        h = g_hash_table_lookup(ht, x->right);
        switch (x->op) {
            case '-':
                x->rOrL = v - x->value;
                break;
            case '/':
                x->rOrL = v / x->value;
                break;
        }
    }
    switch (x->op) {
        case '+':
            x->rOrL = x->value - v;
            break;
        case '*':
            x->rOrL = x->value / v;
            break;
    }
    h->value = x->rOrL;
    h->computed = true;
    return backtrace_human(h);
}

int main() {
    ht = g_hash_table_new(g_str_hash, g_str_equal);
    FILE *f = fopen("/home/tereza/CLionProjects/aoc2022/input.txt", "r");
    while ((lineSize = getline(&linePtr, &n, f)) != -1) {
        linePtr[strcspn(linePtr, "\r\n")] = '\0';
        char *name = calloc(sizeof(char), 5);
        strcpy(name, strtok(linePtr, ":"));
        Node *s = calloc(sizeof(Node), 1);
        s->name = name;
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
        } else if (strcmp(name, "humn") == 0) {
            human = s;
        }

    }

    long eq;
    if (is_human_column(root)) {
        if (root->lHuman) {
            eq = get_value(g_hash_table_lookup(ht, root->right));
        } else {
            eq = get_value(g_hash_table_lookup(ht, root->left));
        }
        root->value = eq + eq;
        root->computed = true;
        root->rOrL = eq;
        long w = backtrace_human(root);
        printf("w = %ld\n", w);
    }

//    printf("--- %ld ---", get_value(root));
    free(linePtr);
    g_hash_table_destroy(ht);

    return 0;
}
