#ifndef ASM_FIBHEAP_H
#define ASM_FIBHEAP_H

/**
 * \brief In fact, a copy of the FibHeap.h file
 * \ref FibHeap.h "See FibHeap.h for more details."
 */

#include <assert.h>
#include <malloc.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdint.h>
#include <string.h>

typedef int64_t Key_t;

typedef struct ASM_FibNode {
    Key_t key;
    struct ASM_FibNode* parent;
    struct ASM_FibNode* left;
    struct ASM_FibNode* right;
    struct ASM_FibNode* child;
    uint64_t degree;
    uint8_t mark;
} ASM_FibNode;

typedef struct ASM_FibHeap {
    ASM_FibNode* min;
    uint64_t size;
    ASM_FibNode** array;
} ASM_FibHeap;

extern ASM_FibHeap* ASM_fibHeapCtor(void);
extern ASM_FibHeap* ASM_fibHeapInit(Key_t key);
extern void ASM_fibHeapDtor(ASM_FibHeap* heap);
extern ASM_FibNode* ASM_fibHeapIns(ASM_FibHeap* heap, Key_t key);
extern void ASM_fibHeapDel(ASM_FibHeap* heap, ASM_FibNode* node);
extern ASM_FibNode* ASM_fibHeapGetMin(const ASM_FibHeap* heap);
extern void ASM_fibHeapExtMin(ASM_FibHeap* heap);
extern void ASM_fibHeapMerge(ASM_FibHeap* first, ASM_FibHeap* second);
extern uint64_t ASM_fibHeapGetSize(const ASM_FibHeap* heap);
extern void ASM_fibHeapOverrideKey(ASM_FibHeap* heap, ASM_FibNode* node, Key_t new_key);

#endif // !ASM_FIBHEAP_H