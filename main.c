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

char **map = NULL;
int rows = 0;
int columns = 0;
int start_row = 0;
int start_column = 0;
int end_row = 0;
int end_column = 0;

typedef struct Node {
    int row;
    int column;
    struct Node *parent;
    struct Node *left;
    struct Node *right;
    struct Node *up;
    struct Node *down;
    bool visited;
    char value;
} Node;

Node **nodes = NULL;

void next_moves(Node *node, Node ***res, int *size);

void path_traversal();

Node *get_sibling(int row, int column, int x, int y) {
    if (row + y < 0 || row + y >= rows || column + x < 0 || column + x >= columns) {
        return NULL;
    }
    return &nodes[row + y][column + x];
};

int main() {
    FILE *f = fopen("/home/tereza/CLionProjects/aoc2022/input.txt", "r");
    char *linePtr = NULL;
    size_t n = 0;
    size_t lineSize;
    while ((lineSize = getline(&linePtr, &n, f)) != -1) {
        linePtr[strcspn(linePtr, "\n\r")] = 0;
        rows++;
        // -delimiter
        columns = (int) strlen(linePtr);

        if (strchr(linePtr, 'S')) {
            start_row = rows - 1;
            start_column = (int) (strchr(linePtr, 'S') - linePtr);
            linePtr[start_column] = 'a';
        }
        if (strchr(linePtr, 'E')) {
            end_row = rows - 1;
            end_column = (int) (strchr(linePtr, 'E') - linePtr);
            linePtr[end_column] = 'z';
        }
//        map = realloc(map, (rows) * sizeof(char *));
//        map[rows - 1] = malloc(columns * sizeof(char));
//        memcpy(map[rows - 1], linePtr, columns);
        nodes = realloc(nodes, (rows) * sizeof(Node *));
        nodes[rows - 1] = malloc(columns * sizeof(Node));
        for (int i = 0; i < columns; ++i) {
            nodes[rows - 1][i].row = rows - 1;
            nodes[rows - 1][i].column = i;
            nodes[rows - 1][i].value = linePtr[i];
            nodes[rows - 1][i].visited = false;
            nodes[rows - 1][i].parent = NULL;
        }

    }
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < columns; ++j) {
            nodes[i][j].left = get_sibling(i, j, -1, 0);
            nodes[i][j].right = get_sibling(i, j, 1, 0);
            nodes[i][j].up = get_sibling(i, j, 0, -1);
            nodes[i][j].down = get_sibling(i, j, 0, 1);
        }
    }
    path_traversal();

    for (int i = 0; i < rows; ++i) {
        free(nodes[i]);
    }
    free(nodes);
    return 0;
}

void path_traversal() {
    int row = start_row;
    int column = start_column;
    int steps = 0;
    Node* *nextMoves = NULL;
    int nextMovesSize = 0;
    Node **currentMoves = NULL;
    int currentMovesSize = 0;
    nextMoves = malloc(sizeof(Node*));
    nextMoves[0] = &nodes[row][column];
    Node * n = &nodes[row][column];
    nextMovesSize = 1;
    while (nextMovesSize > 0 && !(n->row == end_row && n->column == end_column)) {
        // increment step
        steps++;
        // free previous currentMoves (could be NULL, how fix this ?)
        free(currentMoves);
        // update currentMoves
        currentMoves = nextMoves;
        currentMovesSize = nextMovesSize;
        // reset nextMoves
        nextMoves = NULL;
        nextMovesSize = 0;

        while (currentMovesSize > 0 && !(n->row == end_row && n->column == end_column)) {
            currentMovesSize--;
            n = currentMoves[currentMovesSize];
            next_moves(n, &nextMoves, &nextMovesSize);
        }
    }
    printf("Steps: %d", steps);
    while (n->parent != NULL) {
        n->value = '#';
        n = n->parent;
    }
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < columns; ++j) {
            printf("%c", nodes[i][j].value);
        }
        printf("\n");
    }

}

void next_moves(Node *node, Node ***res, int *size) {
    if (node->visited) {
        return;
    }
    node->visited = true;
    Node *moves[4] = {
            node->left,
            node->right,
            node->up,
            node->down
    };
    for (int i = 0; i < 4; ++i) {
        Node *move = moves[i];
        if (move == NULL || move->visited) {
            continue;
        }
        if (move->value - node->value > 1) {
            continue;
        }
        move->parent = node;
        *res = (Node **) realloc(*res, (*size + 1) * sizeof(Node *));
        (*res)[*size] = move;
        *size += 1;
    }
}