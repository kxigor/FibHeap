#include <stdio.h>

#include "./src/ASM_FibHeap/ASM_FibHeap.h"
#include "./src/FibHeap/FibHeap.h"
#include "./src/Graph/Graph.h"
#include "./src/Testing/FibHeapTesting.h"

int32_t main(void) {
    /*Run the tests*/
    //testing();
    printf("sizeof(FibHeap) = %lu\n", sizeof(FibHeap));
    printf("sizeof(FibNode) = %lu\n", sizeof(FibNode));
    FibHeap* asm_heap = ASM_fibHeapCtor();
}