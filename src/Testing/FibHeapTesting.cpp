#include "FibHeapTesting.hpp"
#include "ImprovedVector.hpp"

extern "C" {
#include "../FibHeap/FibHeap.h"
#include "../Graph/Graph.h"
#include "../Stack/Stack.h"
#include "./GlobalHeaderTesting.h"
}

void testing(bool graph) {
    FibHeap* heap = fibHeapCtor();
    ImprovedVector<FibNode*> stk;

    int32_t command = 0;
    while(scanf("%d", &command) == 1) {
        switch (command)
        {
            case FIB_INSERT: {
                int64_t number = 0;
                scanf("%ld", &number);
                stk.push(fibHeapIns(heap, number));
                break;
            }
            case FIB_EXT_MIN: {
                printf("%ld\n", fibHeapGetMin(heap)->key);
                stk.erase(fibHeapGetMin(heap));
                fibHeapExtMin(heap);
                break;
            }
            case FIB_DEL_ALMOST_LAST: {
                printf("%p\n", stk.top().value());
                fibHeapDel(heap, stk.top().value());
                stk.pop();
                break;
            }
            default:
                break;
        }
        if(graph) {
            generateFibHeapDot(heap);
        }
    }

    fibHeapDtor(heap);
}