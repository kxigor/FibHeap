#ifndef FIBHEAP_H
#define FIBHEAP_H

#include <assert.h>
#include <malloc.h>
#include <stdint.h>

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


#define MAX_DEGREE (100)

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
} FibHeap;

FibHeap*    fibHeapCtor();
FibHeap*    fibHeapInit(Key_t key);
void        fibHeapDtor(FibHeap* heap);

/*Merging into the first heap*/
void        fibHeapMerge(FibHeap* first, FibHeap* second);
void        fibHeapDump(const FibHeap* heap);
uint64_t    fibHeapGetSize(const FibHeap* heap);

FibNode*    fibHeapGetMin(const FibHeap* heap);
void        fibHeapExtMin(FibHeap* heap);

FibNode*    fibHeapIns(FibHeap* heap, Key_t key);
void        fibHeapDel(FibNode* node);

#endif // !FIBHEAP_H