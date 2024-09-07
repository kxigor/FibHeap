#include "FibHeap.h"

static inline FibNode*  fibNodeInit(Key_t key);
static inline void      fibNodeUnionLists(FibNode* first, FibNode* second);
static inline void      fibHeapConsolidate(FibHeap* heap);
static inline void      fibNodeLink(FibNode* first, FibNode* second);
static inline void      fibNodeUntie(FibNode* node);
static inline void      fibNodeFixate(FibNode* node);
static inline void      fibHeapCut(FibHeap* heap, FibNode* node);
static inline void      fibHeapСascadingCut(FibHeap* heap, FibNode* node);
static        void      fibNodeDtor(FibNode* node);

FibHeap* fibHeapCtor() {
    FibHeap* heap = calloc(1, sizeof(FibHeap));
    assert(heap != NULL);
    heap->consolidate_array = calloc(CONSOLIDATE_ARRAY_START_SIZE, sizeof(FibNode*));
    assert(heap->consolidate_array != NULL);
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
    free(heap->consolidate_array);
    fibNodeDtor(heap->min);
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

    if(heap->size == 1) {
        free(heap->min);
        heap->min = NULL;
        heap->size = 0;
        return;
    }

    /*Reconnecting the heap elements*/
    /*
        The heap is incorrect at this point, 
        since min does not indicate the minimum element, 
        this will be corrected in Consolidate
    */
    FibNode* delete_node = heap->min; 
    if(heap->min->child != NULL)
        fibNodeUnionLists(heap->min, heap->min->child);

    fibNodeUntie(heap->min);
    heap->min = heap->min->right;

    /*Freeing up the memory of the minimum element*/
    free(delete_node);
    heap->size--;

    /*Restoring the heap*/
    fibHeapConsolidate(heap);
}

void fibHeapOverrideKey(FibHeap* heap, FibNode* node, Key_t new_key) {
    assert(heap != NULL);
    assert(node != NULL);

    node->key = new_key;

    if(node->parent != NULL) {
        if(node->parent->key > node->key) {
            FibNode* parent = node->parent;
            fibHeapCut(heap, node);
            fibHeapСascadingCut(heap, parent);
        }
    }
    if(heap->min->key > node->key)
        heap->min = node;
}

FibNode* fibHeapIns(FibHeap* heap, Key_t key) {
    assert(heap != NULL);
    /*
        I partially repeat what is written in merge, 
        but this is how I try to get rid of creating 
        a new heap in order to improve performance
    */
    FibNode* new_node = fibNodeInit(key);
    if(heap->size == 0) {
        heap->min = new_node;
    } else {
        fibNodeUnionLists(heap->min, new_node);
        if(heap->min->key > new_node->key)
            heap->min = new_node;
    }
    heap->size++;
    return new_node;
}

void fibHeapDel(FibHeap* heap, FibNode* node) {
    assert(heap != NULL);
    assert(node != NULL);
    Key_t new_key = heap->min->key - 1;
    fibHeapOverrideKey(heap, node, new_key);
    fibHeapExtMin(heap);
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

    /*Reconnect the nodes*/
    fibNodeUnionLists(first->min, second->min);

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

static inline void fibNodeUnionLists(FibNode* first, FibNode* second) {
    assert(first != NULL);
    assert(second != NULL);

    /*Carefully reconnect the nodes*/
    first->left->right = second->right;
    second->right->left = first->left;
    first->left = second;
    second->right = first;
}

static inline void fibHeapConsolidate(FibHeap* heap) {
    assert(heap != NULL);

    FibNode* currnet_node = heap->min;
    FibNode* next_node = NULL;
    while(currnet_node != heap->consolidate_array[currnet_node->degree]) {
        currnet_node->parent = NULL;
        /*COMPARATOR*/
        if(heap->min->key > currnet_node->key)
            heap->min = currnet_node;
        /*
            Our task is to go through all the root nodes, 
            if it happens that we hang the current node somewhere, 
            then we will go through the elements a second time. 
            So you need to keep a pointer to the next element
        */
        next_node = currnet_node->right;
        while(heap->consolidate_array[currnet_node->degree] != NULL) {
            /*COMPARATOR*/
            if(currnet_node->key > heap->consolidate_array[currnet_node->degree]->key)
                swap(currnet_node, heap->consolidate_array[currnet_node->degree]);
            /*

            */
            if(next_node == heap->consolidate_array[currnet_node->degree])
                next_node = next_node->right;
            fibNodeLink(currnet_node, heap->consolidate_array[currnet_node->degree]);
            heap->consolidate_array[currnet_node->degree - 1] = NULL;
        }
        heap->consolidate_array[currnet_node->degree] = currnet_node;
        currnet_node = next_node;
    }

    currnet_node = heap->min;
    do {
        heap->consolidate_array[currnet_node->degree] = NULL;
        currnet_node = currnet_node->right;
    } while(currnet_node != heap->min);
}

static inline void fibNodeLink(FibNode* first, FibNode* second) {
    fibNodeUntie(second);
    fibNodeFixate(second);
    
    if(first->child == NULL)
        first->child = second;
    else
        fibNodeUnionLists(first->child, second);

    first->degree++;

    second->parent = first;
}

static inline void fibNodeUntie(FibNode* node) {
    assert(node != NULL);
    //assert(node->parent == NULL);

    node->left->right = node->right;
    node->right->left = node->left;
}

static inline void fibNodeFixate(FibNode* node) {
    assert(node != NULL);
    node->left = node;
    node->right = node;
}

static inline void fibHeapCut(FibHeap* heap, FibNode* node) {
    assert(heap != NULL);
    assert(node != NULL);
    assert(node->parent != NULL);

    node->mark = false;

    node->parent->degree--;

    if(node->parent->child == node) {
        if(node == node->left)
            node->parent->child = NULL;
        else
            node->parent->child = node->left;
    }

    fibNodeUntie(node);
    fibNodeFixate(node);
    node->parent = NULL;
    fibNodeUnionLists(heap->min, node);
}

static inline void fibHeapСascadingCut(FibHeap* heap, FibNode* node) {
    assert(heap != NULL);
    assert(node != NULL);

    FibNode* parent = node->parent;

    while(parent != NULL) {
        if(node->mark == false) {
            node->mark = true;
            break;
        }
        fibHeapCut(heap, node);
        node = parent;
        parent = node->parent;
    }
    node->mark = true;
}

static void fibNodeDtor(FibNode* start_node) {
    if(start_node == NULL)
        return;
    FibNode* node = start_node;
    FibNode* del_node = NULL;
    do {
        fibNodeDtor(node->child);
        del_node = node;
        node = node->right;
        free(del_node);
    } while(node != start_node);
}