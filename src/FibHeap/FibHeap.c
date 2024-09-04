#include "FibHeap.h"

static inline FibNode*  fibNodeInit(Key_t key);
static inline FibHeap*  fibHeapMinChildToHeap(FibHeap* heap);
static inline void      fibHeapConsolidate(FibHeap* heap);

FibHeap* fibHeapCtor() {
    FibHeap* heap = calloc(1, sizeof(FibHeap));
    assert(heap != NULL);
    return heap;
}

FibHeap* fibHeapInit(Key_t key) {
    FibHeap* heap = fibHeapCtor();
    heap->min = fibNodeInit(key);
    heap->size = 1;
    return heap;
}

void fibHeapDtor(FibHeap* heap) {
    assert(heap != NULL);
    free(heap);
}

uint64_t fibHeapGetSize(const FibHeap* heap) {
    assert(heap != NULL);
    return heap->size;
}

FibNode* fibHeapGetMin(const FibHeap* heap) {
    assert(heap != NULL);
    return heap->min;
}

void fibHeapExtMin(FibHeap* heap) {
    assert(heap != NULL);

    /*If the heap is empty, there is nothing to extract*/
    if(heap->size == 0) {
        return;
    }

    /*If there is only one element in the heap, we extract it*/
    if(heap->size == 1) {
        free(heap->min);
        heap->size = 0;
        return;
    }

    /*Creating a separate pile from the children of the minimum node*/
    FibHeap* temp_heap = fibHeapMinChildToHeap(heap->min);

    /*Memorizing the minimum node (for deletion)*/
    FibNode* delete_node = heap->min;

    /*Reconnecting the heap elements*/
    /*
        The heap is incorrect at this point, 
        since min does not indicate the minimum element, 
        this will be corrected in Consolidate
    */
    heap->min->left->right = heap->min->right;
    heap->min->right->left = heap->min->left;
    heap->min = heap->min->right;

    /*Freeing up the memory of the minimum element*/
    free(delete_node);

    /*Сombining the root lists*/
    fibHeapMerge(heap, temp_heap);


}

FibNode* fibHeapIns(FibHeap* heap, Key_t key) {
    assert(heap != NULL);

    FibHeap* temp_heap = fibHeapInit(key); 
    FibNode* temp_node = temp_heap->min;
    fibHeapMerge(heap, temp_heap);
    return temp_node;
}

void fibHeapMerge(FibHeap* first, FibHeap* second) {
    assert(first != NULL);
    assert(second != NULL);

    /*We handle the case if the first heap is empty*/
    if(first->size == 0) {
        first->min = second->min;
        first->size = second->size;
        free(second);
        return;
    }

    /*We handle the case if the second heap is empty*/
    if(second->size == 0) {
        free(second);
        return;
    }

    /*Carefully reconnect the nodes*/
    first->min->left->right = second->min->right;
    second->min->right->left = first->min->left;
    first->min->left = second->min;
    second->min->right = first->min;

    /*COMPARATOR*/
    if(first->min->key > second->min->key)
        first->min = second->min;
    
    /*Do not forget to summarize the size*/
    first->size += second->size;

    /*Freeing up memory after merging*/
    free(second);
}

static inline FibNode* fibNodeInit(Key_t key) {
    FibNode* node = calloc(1, sizeof(FibNode));
    assert(node != NULL);

    /*We create a node and loop on ourselves*/
    node->left = node;
    node->right = node;
    node->degree = 0;
    node->key = key;

    return node;
}

static inline FibHeap* fibHeapMinChildToHeap(FibHeap* heap) {
    assert(heap != NULL);
    assert(heap->min != NULL);
    assert(heap->min->child != NULL);

    FibHeap* new_heap = fibHeapCtor();
    new_heap->min = heap->min->child;
    new_heap->size = heap->min->degree;
    new_heap->min->parent = NULL;
    
    heap->size -= new_heap->size;
    heap->min->child = NULL;

    return new_heap;
}

static inline void fibHeapConsolidate(FibHeap* heap) {
    assert(heap != NULL);

    FibNode* arr_nodes[MAX_DEGREE] = {};
    FibNode* currnet_node = heap->min;
    
    uint64_t degree = 0;

    do {
        degree = currnet_node->degree;
        while(arr_nodes[degree] != NULL) {
            
        }
        currnet_node = currnet_node->right;
    } while(currnet_node != heap->min);

}