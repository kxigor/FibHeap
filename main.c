#include <stdio.h>

#include "./src/ASM_FibHeap/ASM_FibHeap.h"
#include "./src/FibHeap/FibHeap.h"
#include "./src/Graph/Graph.h"
#include "./src/Testing/TestingHeap/TestingHeap.h"

int32_t main(void) {
    /*Run the tests*/
    //testing();
    // printf("sizeof(FibHeap) = %lu\n", sizeof(FibHeap));
    // printf("sizeof(FibNode) = %lu\n", sizeof(FibNode));
    // ASM_FibHeap* asm_heap = ASM_fibHeapInit(10);
    // ASM_fibHeapIns(asm_heap, 100);
    // generateFibHeapDot((FibHeap*)asm_heap);
    // ASM_fibHeapExtMin(asm_heap);
    // generateFibHeapDot((FibHeap*)asm_heap);
    // ASM_fibHeapDtor(asm_heap);

    //ASM_testing();
    ASM_testingHeap(0);
}