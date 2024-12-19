#ifndef FIB_TESTING_H
#define FIB_TESTING_H

/**
 * @brief Enumeration of operations for the Fibonacci heap.
 */
typedef enum FibOperations {
    FIB_INSERT          = 1, /**< Insert a new element into the Fibonacci heap. */
    FIB_EXT_MIN         = 2, /**< Extract the minimum element from the Fibonacci heap. */
    FIB_DEL_ALMOST_LAST = 3, /**< Delete almost the last inserted element from the Fibonacci heap (almost because the improved vector works a little differently (for the sake of complexity O(1), it can move the last one)). */
    FIB_DEC_ALMOST_LAST = 4  /**< Decrease almost the last inserted element from the Fibonacci heap (almost because the improved vector works a little differently (for the sake of complexity O(1), it can move the last one)). */
} FibOperations;

#endif // !FIB_TESTING_H