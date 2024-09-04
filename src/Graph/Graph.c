#include "Graph.h"
#include "../FibHeap/FibHeap.h"

static void generateDot(FibHeap* heap, FibNode* node, FILE* file);

static void generateDot(FibHeap* heap, FibNode* current, FILE* file) {
    FibNode* start_pos = current;
    fprintf(file, "{ rank=same; ");
    do {
        fprintf(file, "  n%p [label=\"%ld\"];\n", current, current->key);
        if(current == heap->min)
            fprintf(file, "  n%p [color=red];\n", current, current->key);
        fprintf(file, "  n%p -> n%p [color=blue];\n", current, current->right);
        fprintf(file, "  n%p -> n%p [color=green];\n", current, current->left);
        current = current->right;
    } while (current != start_pos);
    fprintf(file, "}\n");

    current = start_pos;
    do {
        if(current->child != NULL) {
            fprintf(file, "  n%p -> n%p [color=black];\n", current, current->child);
            generateDot(heap, current->child, file);
        }
        current = current->right;
    } while (current != start_pos);
}


void generateFibHeapDot(FibHeap* heap) {
    FILE* file = fopen("fib_heap.dot", "w");

    if (!file) {
        perror("Unable to open file");
        return;
    }

    fprintf(file, "digraph G {\n");
    fprintf(file, " rankdir=TB;\n");
    fprintf(file, " node [shape=box];\n");

    if (heap->min)
        generateDot(heap, heap->min, file);

    fprintf(file, "}\n");
    fclose(file);
    system("dot -Tpng fib_heap.dot -o fib_heap.png");
}