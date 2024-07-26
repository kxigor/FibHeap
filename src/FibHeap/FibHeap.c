#include "FibHeap.h"

FibHeap* fibHeapCtor() {
    FibHeap* heap = calloc(1, sizeof(FibHeap));
    assert(heap != NULL);
    heap->tmp_arr_size = TMP_ARR_START_SIZE;
    heap->tmp_arr = calloc(heap->tmp_arr_size, sizeof(FibNode));
    assert(heap->tmp_arr != NULL);
    return heap;
}

void fibHeapDtor(FibHeap* heap) {
    assert(heap != NULL);
    assert(heap->tmp_arr != NULL);
    free(heap->tmp_arr);
    free(heap);
}

uint64_t fibHeapGetSize(const FibHeap* heap) {
    assert(heap != NULL);
    return heap->size;
}

FibNode* fibHeapIns(FibHeap* heap, Key_t key) {
    assert(heap != NULL);
}
