#include "./FibHeap/FibHeap.h"
#include "./Graph/Graph.h"
#include "./Testing/FibHeapTesting/FibHeapTesting.hpp"

int32_t main(void) {
#ifdef ASM_FIBHEAP
    ASM_testing(GRAPHBUILDER);
#else
    testing(GRAPHBUILDER);
#endif
}