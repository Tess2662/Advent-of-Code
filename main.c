#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>
#include <sys/types.h>
#include <malloc.h>
#include <stdlib.h>
#include <glib-2.0/glib.h>
#include <ctype.h>

typedef struct node {
    char* key;
} node;
typedef struct hash_set {
    int size;
    int used;
    float th;
    node *nodes;
} hash_set;


void check_bigger(char * max, char * tree, hash_set * hs, int x, int y);
node * h_find(hash_set * hs, char * key);
node * h_add(hash_set * hs, char * key);
void h_rehash(hash_set * hs, int size);
int h_size(hash_set * hs);
hash_set * h_create();
void h_destroy(hash_set * hs);
int h_get_hash(hash_set * hs, char * key);

int main() {
    FILE *f = fopen("/home/tereza/CLionProjects/aoc2022/input.txt", "r");
    char *linePtr = NULL;
    size_t n = 0;
    size_t lineSize;
    hash_set * hs = h_create();
//GHashTable * hs = g_hash_table_new(g_str_hash, g_str_equal);
    char * dyn = NULL;
    int size = 0;
    int l = 0;
    while ((lineSize = getline(&linePtr, &n, f)) != -1) {
        linePtr[strcspn(linePtr, "\r\n")] = '\0';
        char max = 1;
        size++;
        l = (int)strlen(linePtr);
        dyn = realloc(dyn, l * size);
        strcpy(dyn+l*(size-1),linePtr);

        for (int i = 0; i < l; ++i) {
            check_bigger(&max, &linePtr[i], hs, i, size-1);
//            if (linePtr[i] > max) {
////                printf("%c - %c\n", linePtr[i],max);
//                char * s = calloc(20,1);
//                sprintf(s,"%d,%d",i,size-1);
//                h_add(hs,s);
//                max = linePtr[i];
//            }
        }
        max = 1;
        for (int i = l-1; i >= 0; --i) {
            check_bigger(&max, &linePtr[i], hs, i, size-1);
//            if (linePtr[i] > max) {
////                printf("%c - %c\n", linePtr[i],max);
//                char * s = calloc(20,1);
//                sprintf(s,"%d,%d",i,size-1);
//                node * res = h_add(hs,s);
//                if (res == NULL) {
//                    // the other end
//                    break;
//                }
//                max = linePtr[i];
//            }
        }
    }
    for (int i = 0; i < l; ++i) {
        char max = 1;
        for (int j = 0; j < size; ++j) {
            check_bigger(&max, &dyn[j*l+i],hs, i, j);
//            if (dyn[j*l+i] > max) {
////                printf("%c - %c\n",dyn[j*l+i],max);
//                char * s = calloc(20,1);
//                sprintf(s,"%d,%d",i,j);
//                node * res = h_add(hs,s);
//                max = dyn[j*l+i];
//            }
        }
        max = 1;
        for (int j = size-1; j >= 0; --j) {
            check_bigger(&max, &dyn[j*l+i],hs, i, j);
//            if (dyn[j*l+i] > max) {
////                printf("%c - %c\n",dyn[j*l+i],max);
//                char * s = calloc(20,1);
//                sprintf(s,"%d,%d",i,j);
//                node * res = h_add(hs,s);
//                max = dyn[j*l+i];
//            }
        }
    }

    printf("\n%d\n", h_size(hs));
//    printf("\n%d\n",h_size(hs));
//    h_destroy(hs);
    return 0;
}

hash_set * h_create() {
    int size = 2;
    float th = 0.5f;
    hash_set * hs = (hash_set *) malloc(sizeof (hash_set));
    *hs = (hash_set){.size = size, .used = 0, .th = th};
    hs->nodes = calloc(hs->size, sizeof (node));
    return hs;
}

node * h_find(hash_set *hs, char * key) {
    int hash = h_get_hash(hs,key);
    while (hs->nodes[hash].key != NULL && strcmp(hs->nodes[hash].key,key) != 0) {
        hash=(hash+1)%hs->size;
    }
    if (hs->nodes[hash].key != NULL) {
        return &hs->nodes[hash];
    }
    return NULL;
}

node * h_add(hash_set * hs, char * key) {
    if (key == NULL) {
        return NULL;
    }
    if (hs->used >= hs->th * hs->size)
        h_rehash(hs, hs->size * 2);

    int hash = (h_get_hash(hs,key) % hs->size);
//    printf("hash %d\n",hash);
//    printf("size %d\n",hs->size);
//    printf("node %p\n",&hs->nodes[hash]);
    while (hs->nodes[hash].key != NULL && strcmp(hs->nodes[hash].key,key) != 0) {
        hash=(hash+1)%hs->size;
    }
    if (hs->nodes[hash].key != NULL) {
        printf("NULLA;");
        return NULL;
    }
    printf("%s\n",key);
    hs->nodes[hash].key = key;
    hs->used++;
    return &hs->nodes[hash];
}

void h_rehash(hash_set * hs, int size) {
    hash_set old_hs = *hs;
    node * nn = calloc(hs->size,sizeof(node)) ;
    *nn = *hs->nodes;
    hs->size = size;
    hs->used = 0;
    free(hs->nodes);
    hs->nodes = calloc(size,sizeof(node)) ;
    for (int i = 0; i < old_hs.size; ++i) {
        h_add(hs, old_hs.nodes[i].key);
    }
    free(nn);

}

int h_get_hash(hash_set * hs, char * key) {
    return ((int)g_str_hash(key)) % hs->size;
}

void h_destroy(hash_set * hs) {
    free(hs->nodes);
    free(hs);
}

int h_size(hash_set * hs) {
    return hs->used;
}

void check_bigger(char * max, char * tree, hash_set * hs, int x, int y) {

    if (*tree > *max) {
//                printf("%c - %c\n",dyn[j*l+i],max);
        char * s = (char* )malloc(6*sizeof (char));
        sprintf(s,"%d,%d",x,y);
//        node * res = h_add(hs,s);
        h_add(hs,s);
        *max = *tree;
    }
}