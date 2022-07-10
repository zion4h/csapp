#include "cachelab.h"
#include "stdio.h"
#include <getopt.h>
#include <stdlib.h>
#include <unistd.h>

#include "math.h"

/**
 * @author zion
 */

typedef struct block{
    unsigned long tag;
    struct block *next;
}Block;

int s, E, b, v;
char *t;
Block *cache;
void cache_init() {
    int S = 1 << s;
    cache = (Block *) malloc(sizeof(Block) * S);

    for (int i = 0; i < S; i++) {
        cache[i].tag = 0;
        cache[i].next = NULL;
    }
}

int hits, misses, evictions;
void cache_visit(char identifier, unsigned long address, int size) {
    int i = (address >> b) & ((1 << s) - 1);
    unsigned long tag = address >> (s + b);

    if (identifier == 'M') hits++;

    int cnt = 0;
    Block *q = &cache[i];
    Block *p;

    while (q->next != NULL) {
        p = q;
        q = q->next;
        cnt++;

        if (q->tag == tag) {
            hits++;
            // 要将节点运到起始位置
            p->next = q->next;
            q->next = cache[i].next;
            cache[i].next = q;
            return;
        }
    }
    // 没找到
    misses++;
//    printf("set %d add 1 block.\n", i);

    if (cnt == E) {
        free(q);
        p->next = NULL;
        // 弹出老块 腾出空间
        evictions++;
    }

    Block *tmp = (Block *) malloc(sizeof(Block));
    tmp->tag = tag;

    q = cache[i].next;
    cache[i].next = tmp;
    tmp->next = q;
}


void print_usage_info() {
    printf("Usage: ./csim-ref [-hv] -s <num> -E <num> -b <num> -t <file>\n");
    printf("Options:\n");
    printf("  -h         Print this help message.\n");
    printf("  -v         Optional verbose flag.\n");
    printf("  -s <num>   Number of set index bits.\n");
    printf("  -E <num>   Number of lines per set.\n");
    printf("  -b <num>   Number of block offset bits.\n");
    printf("  -t <file>  Trace file.\n");
    printf("\n");
    printf("Examples:\n");
    printf("  linux>  ./csim-ref -s 4 -E 1 -b 4 -t traces/yi.trace\n");
    printf("  linux>  ./csim-ref -v -s 8 -E 2 -b 4 -t traces/yi.trace");
}


int main(int argc, char **argv) {
    int opt;

    while (-1 != (opt = getopt(argc, argv, "hvs:E:b:t:"))) {
        switch (opt) {
            case 'h':
                print_usage_info();
                return -1;
            case 'v':
                v = 1;
                break;
            case 's':
                s = atoi(optarg);
                break;
            case 'E':
                E = atoi(optarg);
                break;
            case 'b':
                b = atoi(optarg);
                break;
            case 't':
                t = optarg;
                break;
            default:
                printf("wrong argument\n");
                print_usage_info();
                return -1;
        }
    }
//    printf("v:%d s:%d E:%d b:%d t:%s\n", v, s, E, b, t);

    cache_init();

    FILE *pFile;
    pFile = fopen(t, "r");
    char identifier;
    unsigned long address;
    int size;

    while (fscanf(pFile, " %c %lx,%d", &identifier, &address, &size) > 0) {
        int hitsOld = hits, missesOld = misses, evictionsOld = evictions;
        switch (identifier) {
            case 'I':
                continue;
                break;
            case 'S':
                cache_visit(identifier, address, size);
                break;
            case 'M':
                cache_visit(identifier, address, size);
                break;
            case 'L':
                cache_visit(identifier, address, size);
                break;
            default:
                printf("%c is ", identifier);
                printf("wrong argument\n");
                return -1;
                break;
        }

        if (v == 1) {
            printf("%c %lx,%d", identifier, address, size);

            if (missesOld < misses) {
                printf(" miss");
            }
            if (hitsOld < hits) {
                printf(" hit");
            }
            if (hitsOld == hits - 2) {
                printf(" hit");
            }
            if (evictionsOld < evictions) {
                printf(" eviction");
            }
            printf("\n");
        }

    }
    fclose(pFile);

    printSummary(hits, misses, evictions);
    return 0;
}

