#ifndef GRAPH_H
#define GRAPH_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/*!
    @defgroup graph Graph Visualization
    @brief Provides functions for visualizing Fibonacci heaps as graphs.

    @warning System dependent library (POSIX), Graphviz must be installed!

    @{
*/

typedef struct FibHeap FibHeap; /**< Forward declaration of the Fibonacci heap structure. */
typedef struct FibNode FibNode; /**< Forward declaration of the Fibonacci node structure. */

/**
 * @brief Generate a DOT file for the given Fibonacci heap.
 *
 * This function creates a DOT representation of the Fibonacci heap and
 * writes it to a file. The DOT format can be used with Graphviz or similar
 * tools to visualize the heap structure.
 *
 * @param heap The Fibonacci heap to be visualized.
 */
void generateFibHeapDot(FibHeap* heap);

#endif // !GRAPH_H