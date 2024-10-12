#include "FibHeapTesting.hpp"

extern "C" {
#include "../../ASM_FibHeap/ASM_FibHeap.h"
#include "../../FibHeap/FibHeap.h"
#include "../../Graph/Graph.h"
#include "../../Stack/Stack.h"
#include "../GlobalHeaderTesting.h"
}

MAKE_TESTING()
// MAKE_TESTING(ASM_)

void ASM_testing(bool graph) {
  ASM_FibHeap* heap = ASM_fibHeapCtor();
  ImprovedVector<ASM_FibNode*> stk;
  int32_t command = 0;
  while (scanf("%d", &command) == 1) {
    switch (command) {
      case FIB_INSERT: {
        int64_t number = 0;
        scanf("%ld", &number);
        stk.push(ASM_fibHeapIns(heap, number));
        break;
      }
      case FIB_EXT_MIN: {
        printf("%ld\n", ASM_fibHeapGetMin(heap)->key);
        stk.erase(ASM_fibHeapGetMin(heap));
        ASM_fibHeapExtMin(heap);
        break;
      }
      case FIB_DEL_ALMOST_LAST: {
        ASM_fibHeapDel(heap, stk.top().value());
        stk.pop();
        break;
      }
      default:
        break;
    }
    if (graph) {
      generateFibHeapDot(reinterpret_cast<FibHeap*>(heap));
    }
  }
  ASM_fibHeapDtor(heap);
}