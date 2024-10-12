#ifndef FIBHEAP_H
#define FIBHEAP_H

#include <assert.h>
#include <malloc.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdint.h>
#include <string.h>

#include "../Stack/Stack.h"

/*!
    \defgroup fibheap Fibonacci Heap
    \brief A module for managing and operating on Fibonacci heaps.

    Information about the Fibonacci heap can be found here: https://en.wikipedia.org/wiki/Fibonacci_heap

    @{
*/

/**
 * @brief Maximum size of the heap merge array.
 * @warning The program does not provide overflow checks for this array!
 */
#define FIB_ARRAY_START_SIZE (500)

/**
 * @brief Typedef for key type in Fibonacci heap.
 * 
 * Represents the key type used for elements in the Fibonacci heap.
 * It is defined as a 64-bit integer (`int64_t`).
 */
typedef int64_t Key_t;

/**
 * @brief Structure representing a node in a Fibonacci heap.
 * 
 * This structure defines the elements of a node in a Fibonacci heap.
 */
typedef struct FibNode {
    /** @brief The key value associated with the node. */
    Key_t key;
    /** @brief Pointer to the parent node in the heap. */
    struct FibNode* parent;
    /** @brief Pointer to the left sibling node in the circular doubly linked list. */
    struct FibNode* left;
    /** @brief Pointer to the right sibling node in the circular doubly linked list. */
    struct FibNode* right;
    /** @brief Pointer to the first child node of this node. */
    struct FibNode* child;
    /** @brief The number of children of this node. */
    uint64_t degree;
    /** @brief A boolean flag indicating whether the node has lost a child since it was last made a child of another node. */
    uint8_t mark;
} FibNode;
/**
 * @brief Represents a Fibonacci heap.
 */
typedef struct FibHeap {
    FibNode* min; /**< Pointer to the minimum node in the heap. */
    uint64_t size; /**< Number of nodes in the heap. */
    FibNode** array; /**< Array for consolidating nodes during heap operations. */
} FibHeap;

/**
 * @brief Create an empty Fibonacci heap.
 *
 * @return A pointer to the newly created Fibonacci heap.
 */
FibHeap* fibHeapCtor(void);

/**
 * @brief Create a Fibonacci heap with an initialized element.
 *
 * @param key The key for the initial node.
 * @return A pointer to the newly created Fibonacci heap with an initial node.
 */
FibHeap* fibHeapInit(Key_t key);

/**
 * @brief Destroy the Fibonacci heap and free all associated resources.
 *
 * @param heap The heap to be destroyed.
 */
void fibHeapDtor(FibHeap* heap);

/**
 * @brief Insert a node with the given key into the Fibonacci heap.
 *
 * @param heap The heap to insert the node into.
 * @param key The key for the node to be inserted.
 * @return A pointer to the newly inserted node.
 * @warning It is not recommended to modify the node, as it may affect the heap's validity.
 */
FibNode* fibHeapIns(FibHeap* heap, Key_t key);

/**
 * @brief Remove a specific node from the Fibonacci heap.
 *
 * @param heap The heap from which the node will be removed.
 * @param node The node to be removed.
 */
void fibHeapDel(FibHeap* heap, FibNode* node);

/**
 * @brief Get the minimum node of the Fibonacci heap.
 * 
 * @param heap The heap from which the minimum node is retrieved.
 * @return A pointer to the minimum node of the heap.
 * @warning It is not recommended to modify the node, as it may affect the heap's validity.
 */
FibNode* fibHeapGetMin(const FibHeap* heap);

/**
 * @brief Extract the minimum node from the Fibonacci heap.
 *
 * @param heap The heap from which the minimum node will be extracted.
 */
void fibHeapExtMin(FibHeap* heap);

/**
 * @brief Merge two Fibonacci heaps.
 *
 * @param first The heap into which the second heap will be merged.
 * @param second The heap to be merged into the first heap.
 * @warning The second heap is merged into the first one, and the second heap is destroyed.
 */
void fibHeapMerge(FibHeap* first, FibHeap* second);

/**
 * @brief Get the size of the Fibonacci heap.
 *
 * @param heap The heap whose size is to be retrieved.
 * @return The number of nodes in the heap.
 */
uint64_t fibHeapGetSize(const FibHeap* heap);

/**
 * @brief Change the key of a specific node in the Fibonacci heap.
 *
 * The validity of the heap will be preserved after the key change.
 *
 * @param heap The heap containing the node.
 * @param node The node whose key will be changed.
 * @param new_key The new key to be assigned to the node.
 */
void fibHeapOverrideKey(FibHeap* heap, FibNode* node, Key_t new_key);

/*! @} */

#endif // !FIBHEAP_H