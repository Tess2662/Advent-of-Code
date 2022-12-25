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

typedef struct {
    int resources[4];
    int curRes;
    int neededMaterial[4][3];
    int bots[4];
} Blueprint;

typedef struct {
    int mRounds;
    int mResource;
    int mParent;
} Bottleneck;

char *linePtr = NULL;
size_t n = 0;
size_t lineSize;
int count = 0;
Blueprint bp;
int btc = 0;
Bottleneck bottlenecks[4];

char *find_space(int c) {
    int s = 0;
    int ptr = 0;
    while (linePtr[ptr] != '\0' && s < c) {
        if (linePtr[ptr] == ' ') {
            s++;
        }
        ptr++;
    }
    return linePtr + ptr;
}

int get_rounds(int bot, int material, int ownedRes, bool botAdded) {
    if (bp.neededMaterial[bot][material] == 0) return 0;
    assert(bp.bots[material] != 0);
    return (int) (
            (float) (bp.neededMaterial[bot][material] - ownedRes + botAdded) / (float) (bp.bots[material] + botAdded) +
            0.98);
}

Bottleneck get_bottleneck(int bot) {
    Bottleneck res = {-1, -1, bot};
    for (int i = 0; i < 3; ++i) {
        int rounds = get_rounds(bot, i, bp.resources[i], false);
        if (rounds > res.mRounds) {
            res.mRounds = rounds;
            res.mResource = i;
        }
    }
    return res;
}

void create_bot(int bot) {
    for (int i = 0; i < 3; ++i) {
        bp.resources[i] -= bp.neededMaterial[bot][i];
    }
    bp.bots[bot]++;
}

int will_make_it_change(int bot1, int bot2) {
    int mr = 0;
    for (int j = 0; j < 3; ++j) {
        int ownedRes = bp.resources[j] - bp.neededMaterial[bot1][j];
        int rounds = get_rounds(bot2, j, ownedRes, j == bot1);
        mr = rounds > mr ? rounds : mr;
    }
    return mr;
}


int main() {
    FILE *f = fopen("/home/tereza/CLionProjects/aoc2022/input.txt", "r");
    int sum = 0;
    while ((lineSize = getline(&linePtr, &n, f)) != -1) {
        count++;
        bp = (Blueprint) {{0, 0, 0, 0}, 0, {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}}, {1, 0, 0, 0}};
        int c;
        c = (int) strtol(find_space(6), NULL, 10);
        bp.neededMaterial[0][0] = c;
        c = (int) strtol(find_space(12), NULL, 10);
        bp.neededMaterial[1][0] = c;
        c = (int) strtol(find_space(18), NULL, 10);
        bp.neededMaterial[2][0] = c;
        c = (int) strtol(find_space(21), NULL, 10);
        bp.neededMaterial[2][1] = c;
        c = (int) strtol(find_space(27), NULL, 10);
        bp.neededMaterial[3][0] = c;
        c = (int) strtol(find_space(30), NULL, 10);
        bp.neededMaterial[3][2] = c;


        for (int i = 0; i < 24; ++i) {
            Bottleneck bns[4];
            btc = 0;
            int neck = bp.curRes >= 3 ? bp.curRes : bp.curRes + 1;
            while (true) {
                Bottleneck bn = get_bottleneck(neck);
                if (bn.mRounds == 0) {
                    bool good = true;
                    int very_good = btc == 0;
                    for (int j = btc - 1; j >= 0; --j) {
                        if (will_make_it_change(neck, bns[btc - 1].mParent) > bns[btc - 1].mRounds) {
                            good = false;
                        }
                        if (will_make_it_change(neck, bns[btc - 1].mParent) < bns[btc - 1].mRounds) {
                            very_good = true;
                        }
                    }
                    if (good && very_good) {
                        if (neck == bp.curRes+1) {
                            bp.curRes++;
                        }
                        create_bot(neck);
                        bp.resources[neck]--;
                    }
                    break;
                }
                bns[btc] = bn;
                btc++;
                neck = bn.mResource;
                if (btc > 3) {
                    break;
                }
            }
            for (int j = 0; j < 4; ++j) {
                bp.resources[j] += bp.bots[j];
            }
        }

        sum += bp.resources[3] * count;

    }

    printf("%d", sum);
    free(linePtr);

    return 0;
}
