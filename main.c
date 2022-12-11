#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>
#include <sys/types.h>
#include <malloc.h>
#include <stdlib.h>
#include <glib-2.0/glib.h>

void wtf(gpointer p, gpointer p1, gpointer p3) {
    printf("%s %s %s\n", (char *)p, (char *)p1, (char *)p3);
}
gboolean pred(gpointer a, gpointer b, gpointer c) {
    printf("%s %s %s\n", a,b,c);
    return (strcmp(a, b) == 0);
}

int main() {
    FILE *f = fopen("/home/tereza/CLionProjects/aoc2022/input.txt", "r");
    char *linePtr = NULL;
    size_t n = 0;
    size_t lineSize;
    GHashTable *ht = g_hash_table_new(g_str_hash,g_str_equal);
    int step = 0;
    int hx = 0, hy = 0, tx = 0,ty = 0;
    char* dir;
    while ((lineSize = getline(&linePtr, &n, f)) != -1) {
       dir = strtok(linePtr, " ");
       step =(int) strtol(strtok(NULL, " "), NULL, 10);
       if (strspn(dir, "RL"))
       {
           int sign = (*dir - 'O');
           sign = sign/abs(sign);
           hx += sign * step;
           for (int i = 1; abs(hx - tx) > 1; ++i) {
               tx +=sign;
               ty = hy;
               char *s = calloc(1,20);
               sprintf(s,"%d,%d",tx,hy);
               gpointer u;

//               g_hash_table_insert(ht,  s, s);
                   g_hash_table_add(ht,  s);
           }
       }else {
           int sign = ('O' - *dir);
           sign = sign/abs(sign);
          hy += sign* step;
           for (int i = 1; abs(hy - ty) > 1; ++i) {
               ty += sign;
               tx = hx;
               char *s = calloc(1,20);
               sprintf(s,"%d,%d",hx,ty);

                   g_hash_table_add(ht, s);

           }
       }
       unsigned int xx = g_hash_table_size(ht);
//        printf("%d", xx);

    }
    GList * l = g_hash_table_get_keys(ht);

    gpointer u = NULL;
    g_hash_table_foreach(ht, wtf, NULL);
    printf("%d interesting", g_hash_table_size(ht));
    return 0;
}
