#ifndef FIB_GENERATOR_H
#define FIB_GENERATOR_H

/**
 * @brief Starts the testing process using a Fibonacci heap and stack.
 *
 * This function reads commands from standard input and performs operations 
 * on the Fibonacci heap and stack based on the entered command. 
 * Supported commands include:
 * - Inserting an element into the heap (FIB_INSERT)
 * - Extracting the minimum element from the heap (FIB_EXT_MIN)
 * - Deleting the last inserted element (FIB_DEL_LAST)
 */

void testing(bool graph);
void ASM_testing(bool graph);

#define MAKE_TESTING(prefix) \
  void prefix##testing(bool graph) { \
    prefix##FibHeap* heap = prefix##fibHeapCtor(); \
    ImprovedVector<prefix##FibNode*> stk; \
    int32_t command = 0; \
    while (scanf("%d", &command) == 1) { \
      switch (command) { \
        case FIB_INSERT: { \
          int64_t number = 0; \
          scanf("%ld", &number); \
          stk.push(prefix##fibHeapIns(heap, number)); \
          break; \
        } \
        case FIB_EXT_MIN: { \
          printf("%ld\n", prefix##fibHeapGetMin(heap)->key); \
          stk.erase(prefix##fibHeapGetMin(heap)); \
          prefix##fibHeapExtMin(heap); \
          break; \
        } \
        case FIB_DEL_ALMOST_LAST: { \
          prefix##fibHeapDel(heap, stk.top().value()); \
          stk.pop(); \
          break; \
        } \
        default: \
          break; \
      } \
      if (graph) { \
        generateFibHeapDot(heap); \
      } \
    } \
    prefix##fibHeapDtor(heap); \
  }

#endif // !FIB_GENERATOR_H