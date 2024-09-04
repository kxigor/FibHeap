#include <stdio.h>

#include "./src/FibHeap/FibHeap.h"
#include "./src/Graph/Graph.h"

int32_t main(void) {
    FibHeap* heap = fibHeapCtor();
    int32_t command = 0;
    FibNode* last_ins = NULL;
    while(scanf("%d", &command) == 1) {
        switch (command)
        {
            case 1:
                int64_t number = 0;
                scanf("%ld", &number);
                last_ins = fibHeapIns(heap, number);
                break;
            case 2:
                fibHeapExtMin(heap);
                break;
            case 3:
                fibHeapDel(heap, last_ins);
                break;
            default:
                break;
        }
        generateFibHeapDot(heap);
    }
    printf("%s\n", "Exit ...");
}