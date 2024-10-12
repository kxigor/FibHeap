#include "FibHeap.h"

/**
 * @brief Swap the values of two variables.
 *
 * This macro swaps the values of two variables using a temporary variable.
 * It uses the `__typeof__` operator to ensure that the temporary variable
 * is of the same type as the input variables.
 *
 * @param a The first variable to be swapped.
 * @param b The second variable to be swapped.
 *
 * Example usage:
 * @code
 * int x = 5, y = 10;
 * swap(x, y);
 * // Now x is 10 and y is 5
 * @endcode
 * @warning a and b must be of the same type!
 */
#define swap(a, b)\
    do {\
        __typeof__ (a) temp = (a);\
        a = b;\
        b = temp;\
    } while (0)\

/*!
 * \defgroup techdetails Technical Details
 * \brief Detailed technical information for developers.
 * @{
 */

/**
 * @brief Create a new FibNode with an initialized element.
 * 
 * @param key The key for the new node.
 * @return A pointer to the newly created FibNode with an initial key.
 */
static inline FibNode* fibNodeInit(Key_t key);

/**
 * @brief Destroy a Fibonacci node and free its resources.
 *
 * @param node The node to be destroyed.
 */
static inline void fibNodeDtor(FibNode* node);

/**
 * @brief Union two FibNode lists.
 * 
 * @param first The first node to union.
 * @param second The second node to union.
 */
static inline void fibNodeUnionLists(FibNode* first, FibNode* second);

/**
 * @brief Consolidate the Fibonacci heap.
 * 
 * @param heap The Fibonacci heap to be consolidated.
 */
static inline void fibHeapConsolidate(FibHeap* heap);

/**
 * @brief Link the second node to the first node childs.
 * 
 * @param first The node to which they link.
 * @param second The node that is linked.
 */
static inline void fibNodeLink(FibNode* first, FibNode* second);

/**
 * @brief Untie the node from its list.
 * 
 * @param node The node to be untied.
 * @warning If there is only one node in the list, then a bug will occur
 */
static inline void fibNodeUntie(FibNode* node);

/**
 * @brief Loop the node on itself.
 * 
 * @param node The node that will be looped.
 */
static inline void fibNodeFixate(FibNode* node);

/**
 * @brief Procedure cutting node from its list and link to root list.
 * 
 * @param heap The heap from which the node will be cut and link into the root list.
 * @param node The node to be cut.
 */
static void fibHeapCut(FibHeap* heap, FibNode* node);

/**
 * @brief Perform cascading cut on a node in the Fibonacci heap.
 * 
 * @param The Fibonacci heap in which the cascading cut is performed.
 * @param node The starting node for the cascading cut.
 */
static void fibHeapСascadingCut(FibHeap* heap, FibNode* node);

FibHeap* fibHeapCtor(void) {
    FibHeap* heap = calloc(1, sizeof(FibHeap));
    assert(heap != NULL);

    heap->array = calloc(FIB_ARRAY_START_SIZE, sizeof(FibNode*));
    assert(heap->array != NULL);

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

    /*Non-recursive node deletion*/
    fibNodeDtor(heap->min);

    free(heap->array);
    free(heap);
}

FibNode* fibHeapIns(FibHeap* heap, Key_t key) {
    assert(heap != NULL);

    /*
        I partially repeat what is written in merge, 
        but this is how I try to get rid of creating 
        a new heap in order to improve performance.
    */

    /*Creating a node with a key*/
    FibNode* new_node = fibNodeInit(key);
    /*If heap empty then link new minimum*/
    if(heap->size == 0) {
        heap->min = new_node;
    }
    /*If heap not empty then combining the node lists*/
    else {
        fibNodeUnionLists(heap->min, new_node);
        /*Do not forget to update the minimum*/
        if(heap->min->key > new_node->key)
            heap->min = new_node;
    }
    /*The heap size has been increased by one*/
    heap->size++;

    return new_node;
}

void fibHeapDel(FibHeap* heap, FibNode* node) {
    assert(heap != NULL);
    assert(node != NULL);

    /*
        To delete a node, we make its,
        value less than the minimum 
        (there may be bugs if the minimum 
        is already the minimum, i.e. INT64_MIN).
    */
    Key_t new_key = heap->min->key - 1;

    /*Overwriting with a new key*/
    fibHeapOverrideKey(heap, node, new_key);
    /*We extract the minimum (respectively, our node)*/
    fibHeapExtMin(heap);
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

    /*Let's process one element separately to clear heap->min*/
    if(heap->size == 1) {
        free(heap->min);
        heap->min = NULL;
        heap->size = 0;
        return;
    }

    /*Memorizing the node to delete*/
    FibNode* delete_node = heap->min;

    /*
        Reconnecting the heap elements.
        We union the children at the root.
    */
    if(heap->min->child != NULL)
        fibNodeUnionLists(heap->min, heap->min->child);
    /*
        The heap is incorrect at this point, 
        since min does not indicate the minimum element, 
        this will be corrected in Consolidate.
    */

    /*Untying the minimum*/
    fibNodeUntie(heap->min);
    /*
        We have untied the minimum, 
        but we have not changed its connections, 
        so this entry makes sense.
    */
    heap->min = heap->min->right;

    /*Freeing up the memory of the minimum element*/
    free(delete_node);
    /*The heap size has been decreased by one*/
    heap->size--;

    /*Restoring the heap*/
    fibHeapConsolidate(heap);
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

    /*Unioning the root lists of two heaps*/
    fibNodeUnionLists(first->min, second->min);

    /*Do not forget to update the minimum*/
    if(first->min->key > second->min->key)
        first->min = second->min;
    
    /*Do not forget to summarize the size*/
    first->size += second->size;

    /*Freeing up memory after merging*/
    free(second);
}

uint64_t fibHeapGetSize(const FibHeap* heap) {
    assert(heap != NULL);
    return heap->size;
}

void fibHeapOverrideKey(FibHeap* heap, FibNode* node, Key_t new_key) {
    assert(heap != NULL);
    assert(node != NULL);

    node->key = new_key;

    /*We apply cascading cutting of children up the lists*/
    if(node->parent != NULL) {
        if(node->parent->key > node->key) {
            FibNode* parent = node->parent;
            fibHeapCut(heap, node);
            fibHeapСascadingCut(heap, parent);
        }
    }

    /*Do not forget to update the minimum*/
    if(heap->min->key > node->key)
        heap->min = node;
}

static inline FibNode* fibNodeInit(Key_t key) {
    FibNode* node = calloc(1, sizeof(FibNode));
    assert(node != NULL);

    /*We create a node and loop on ourselves*/
    node->key = key;
    node->left = node;
    node->right = node;

    return node;
}

static inline void fibNodeDtor(FibNode* node) {
    if(node == NULL)
        return;

    /*
        Implementation via the stack, 
        so as not to overflow the call stack with recursion.
    */

    /*Creating a stack*/
    Stack* stk = stackCtor();
    assert(stk != NULL);

    /*The initial node to figure out when to end the cycle*/ 
    FibNode* first_node = node;

    for(;;) {
        /*If there is a child, we remember it*/
        if(node->child != NULL)
            stackPush(stk, node->child);

        stackPush(stk, node->right);
        free(node);
        node = stackTop(stk);
        stackPop(stk);

        /*
            If we find that we have gone through the 
            entire list, we check the stack. If the 
            stack is empty, we have cleared everything, 
            we exit the loop. If the stack is not empty, 
            we switch to the child and go through it again. 
            It is important to note that with 
            this implementation, the sequence of children is 
            correct and does not need to be tracked.
        */
        if(node == first_node) {
            if(!stackSize(stk))
                break;
            node = stackTop(stk);
            stackPop(stk);
            first_node = node;
        }
    }

    /*Destroying the stack*/
    stackDtor(stk);
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
    FibNode* end_node = currnet_node;
    do {
        currnet_node->parent = NULL;
        if(heap->min->key > currnet_node->key) {
            heap->min= currnet_node;
        }
        FibNode* next_node = currnet_node->right;
        while (heap->array[currnet_node->degree] != NULL) {
            if(currnet_node->key > heap->array[currnet_node->degree]->key) {
                swap(currnet_node, heap->array[currnet_node->degree]);
            }
            if(end_node == heap->array[currnet_node->degree]) {
                end_node = end_node->right;
            }
            if(next_node == heap->array[currnet_node->degree]) {
                next_node = next_node->right;
            }
            if(heap->min == heap->array[currnet_node->degree]) {
                heap->min = currnet_node;
            }
            fibNodeLink(currnet_node, heap->array[currnet_node->degree]);
            heap->array[currnet_node->degree - 1] = NULL;
        }
        heap->array[currnet_node->degree] = currnet_node;
        currnet_node = next_node;
    } while(currnet_node != end_node);

    currnet_node = heap->min;
    do {
        heap->array[currnet_node->degree] = NULL;
        currnet_node = currnet_node->right;
    } while(currnet_node != heap->min);
}

static inline void fibNodeLink(FibNode* first, FibNode* second) {
    /*We untie and loop the second node*/
    fibNodeUntie(second);
    fibNodeFixate(second);
    
    /*
        If there were no children, 
        then the first one appears. 
        If there were children, 
        we combine the lists of nodes.
    */
    if(first->child == NULL)
        first->child = second;
    else
        fibNodeUnionLists(first->child, second);

    first->degree++;

    /*Do not forget to update the parent*/
    second->parent = first;
}

static inline void fibNodeUntie(FibNode* node) {
    assert(node != NULL);

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

    /*Updating children from a parent*/
    if(node->parent->child == node) {
        if(node == node->left)
            node->parent->child = NULL;
        else
            node->parent->child = node->left;
    }

    /*We untie and loop the second node*/
    fibNodeUntie(node);
    fibNodeFixate(node);

    /*Do not forget to update the parent*/
    node->parent = NULL;

    /*Moving the cut element to the root list*/
    fibNodeUnionLists(heap->min, node);
}

static inline void fibHeapСascadingCut(FibHeap* heap, FibNode* node) {
    assert(heap != NULL);
    assert(node != NULL);

    /*
        We remember the parent because in 
        the process of cutting it will disappear.
    */
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

/*! @} */
