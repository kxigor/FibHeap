#include "FibHeapTesting.h"

/**
 * @brief Deletes a pointer from the stack.
 *
 * This function searches for a pointer in the stack and removes it, shifting
 * the remaining elements one position to the left. If the pointer is not found,
 * the function returns an error.
 *
 * @param stk Pointer to the stack from which the element needs to be removed.
 * @param ptr Pointer to the element that needs to be deleted.
 * @return int8_t Result code of the operation:
 *         - SE_ALL_OK: Successful deletion.
 *         - SE_POINTER_ERR: Pointer not found in the stack.
 *
 * @pre stk must not be NULL, stk->data must not be NULL, ptr must not be NULL.
 */
static inline int8_t kludge_stackDelete(Stack* stk, void* ptr);

void testing() {
    FibHeap* heap = fibHeapCtor();
    Stack* stk = stackCtor();

    int32_t command = 0;
    while(scanf("%d", &command) == 1) {
        switch (command)
        {
            case FIB_INSERT: {
                int64_t number = 0;
                scanf("%ld", &number);
                stackPush(stk, fibHeapIns(heap, number));
                break;
            }
            case FIB_EXT_MIN: {
                printf("%ld\n", fibHeapGetMin(heap)->key);
                kludge_stackDelete(stk, fibHeapGetMin(heap));
                fibHeapExtMin(heap);
                break;
            }
            case FIB_DEL_LAST: {
                fibHeapDel(heap, stackTop(stk));
                stackPop(stk);
                break;
            }
            default:
                break;
        }
        /*
            You can uncomment for beautiful graphs, 
            but the tests will run indefinitely.
        */
        //generateFibHeapDot(heap);
    }

    stackDtor(stk);
    fibHeapDtor(heap);
}

static inline int8_t kludge_stackDelete(Stack *stk, void* ptr) {
    assert(stk != NULL);
    assert(stk->data != NULL);
    assert(ptr != NULL);

    for(uint64_t pos = 0; pos < stk->size; pos++) {
        if(stk->data[pos] == ptr) {
            for(uint64_t new_pos = pos; new_pos < stk->size - 1; new_pos++)
                stk->data[new_pos] = stk->data[new_pos + 1];
            stk->size--;
            return SE_ALL_OK;
        }
    }

    return SE_POINTER_ERR;
}