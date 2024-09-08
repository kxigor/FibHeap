#ifndef FIB_TESTING_H
#define FIB_TESTING_H

/**
 * @brief Enumeration of operations for the Fibonacci heap.
 */
typedef enum FibOperations {
    FIB_INSERT      = 1, /**< Insert a new element into the Fibonacci heap. */
    FIB_EXT_MIN     = 2, /**< Extract the minimum element from the Fibonacci heap. */
    FIB_DEL_LAST    = 3, /**< Delete the last inserted element from the Fibonacci heap. */
} FibOperations;

#endif // !FIB_TESTING_H