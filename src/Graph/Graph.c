#include "Graph.h"

#include "../FibHeap/FibHeap.h"

/*!
 * \defgroup techdetails Technical Details
 * \brief Detailed technical information for developers.
 * @{
 */

/**
 * @brief Helper function to generate DOT representation of a Fibonacci heap node.
 *
 * This static function recursively generates the DOT representation for
 * the nodes in the heap starting from the given node and writes it to
 * the specified file.
 *
 * @param heap The Fibonacci heap being visualized.
 * @param node The starting node for the DOT generation.
 * @param file The file to which the DOT data will be written.
 */
static void generateDot(FibHeap* heap, FibNode* node, FILE* file);

void generateFibHeapDot(FibHeap* heap) {
    FILE* file = fopen("fib_heap.dot", "w");

    if (file == NULL) {
        perror("Unable to open file!");
        return;
    }

    fprintf(file, "digraph G {\n");
    fprintf(file, "labelloc=\"t\"\n");
    fprintf(file, "label=\"ASM Fibonacci heap by KXI\"\n");
    fprintf(file, "rankdir=TB;\n");
    fprintf(file, "node [shape=box];\n");

    if (heap->min)
        generateDot(heap, heap->min, file);

    fprintf(file, "}\n");
    fclose(file);
    system("dot -Tpng fib_heap.dot -o fib_heap.png");
}

static void generateDot(FibHeap* heap, FibNode* start_pos, FILE* file) {

    FibNode* current = start_pos;

    fprintf(file, "{ rank=same; ");
    do {
        fprintf(file, "n%p [label=\"key=%ld\naddr=%p\"];\n", current, current->key, current);
        if(current == heap->min)
            fprintf(file, "n%p [color=red];\n", current);
        fprintf(file, "n%p -> n%p [color=blue];\n", current, current->right);
        fprintf(file, "n%p -> n%p [color=green];\n", current, current->left);
        current = current->right;
    } while (current != start_pos);
    fprintf(file, "}\n");

    current = start_pos;
    do {
        if(current->child != NULL) {
            fprintf(file, "n%p -> n%p [color=black];\n", current, current->child);
            generateDot(heap, current->child, file);
        }
        current = current->right;
    } while (current != start_pos);
}

/*! @} */