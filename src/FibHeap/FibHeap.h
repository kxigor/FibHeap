#ifndef FIBHEAP_H
#define FIBHEAP_H

#include <assert.h>
#include <malloc.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#define min(a, b) ({\
    __typeof__ (a) _a = (a);\
    __typeof__ (b) _b = (b);\
    (_a > _b ? _a : _b);\
})

#define swap(a, b)\
    do {\
        __typeof__ (a) temp = (a);\
        a = b;\
        b = temp;\
    } while (0)\


#define CONSOLIDATE_ARRAY_START_SIZE (50000)

typedef int64_t Key_t;

typedef struct FibNode {
    Key_t key;
    struct FibNode* parent;
    struct FibNode* left;
    struct FibNode* right;
    struct FibNode* child;
    uint64_t degree;
    uint8_t mark;
} FibNode;

typedef struct FibHeap {
    FibNode* min;
    uint64_t size;

    FibNode** consolidate_array;
} FibHeap;

FibHeap*    fibHeapCtor();
FibHeap*    fibHeapInit(Key_t key);
void        fibHeapDtor(FibHeap* heap);

/*Merging into the first heap*/
void        fibHeapMerge(FibHeap* first, FibHeap* second);
uint64_t    fibHeapGetSize(const FibHeap* heap);

FibNode*    fibHeapGetMin(const FibHeap* heap);
void        fibHeapExtMin(FibHeap* heap);

void        fibHeapOverrideKey(FibHeap* heap, FibNode* node, Key_t new_key);

FibNode*    fibHeapIns(FibHeap* heap, Key_t key);
void        fibHeapDel(FibHeap* heap, FibNode* node);

#endif // !FIBHEAP_H