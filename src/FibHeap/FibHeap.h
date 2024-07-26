#ifndef FIBHEAP_H
#define FIBHEAP_H

#include <assert.h>
#include <malloc.h>
#include <stdint.h>

#define TMP_ARR_START_SIZE (1000)

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

    FibNode* tmp_arr;
    uint64_t tmp_arr_size;
} FibHeap;

FibHeap*    fibHeapCtor();
void        fibHeapDtor(FibHeap* heap);
void        fibHeapDump(const FibHeap* heap);
uint64_t    fibHeapGetSize(const FibHeap* heap);

FibNode*    fibHeapGetMin(const FibHeap* heap);
void        fibHeapExtMin(FibHeap* heap);

FibNode*    fibHeapIns(FibHeap* heap, Key_t key);
void        fibHeapDel(FibNode* node);

#endif // !FIBHEAP_H