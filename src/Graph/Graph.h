#ifndef GRAPH_H
#define GRAPH_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct FibHeap FibHeap;
typedef struct FibNode FibNode;

void generateFibHeapDot(FibHeap* heap);

#endif // !GRAPH_H