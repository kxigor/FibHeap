#include <stdio.h>

#include "./src/FibHeap/FibHeap.h"
#include "./src/Graph/Graph.h"

int32_t main(void) {
    FibHeap* heap = fibHeapCtor();
    int32_t command = 0;
    while(scanf("%d", &command) == 1) {
        switch (command)
        {
            case 1:
                int64_t number = 0;
                scanf("%ld", &number);
                fibHeapIns(heap, number);
                break;
            case 2:
                fibHeapExtMin(heap);
                break;
            default:
                break;
        }
        generateFibHeapDot(heap);
    }
    printf("%s\n", "Exit ...");
}