#include "Stack.h"

/**
 * @brief Checks a condition and returns an error code if the condition is false.
 * 
 * @param condition The condition to be checked.
 * @param ret The error code to be returned if the condition is false.
 */
#define errCheck(condition, ret) \
    do { \
        if(!(condition)) \
            return ret; \
    } while(0);
    
/**
 * @def stkRellocate(stk)
 * @brief Reallocates memory for the stack's data array.
 * 
 * @param stk A pointer to the stack whose data array is to be reallocated.
 * 
 * @note The macro uses the `errCheck` macro to handle memory allocation errors.
 */
#define stkRellocate(stk)  \
    do { \
        void* new_data = realloc(stk->data, stk->capacity * sizeof(void*)); \
        errCheck(new_data, SE_MALLOC_ERR); \
        stk->data = new_data; \
    } while (0);
    

Stack* stackCtor() {
    Stack* stk = malloc(sizeof(Stack));
    errCheck(stk, NULL);

    stk->size = 0;
    stk->capacity = STK_START_SIZE;
    stk->data = calloc(stk->capacity, sizeof(void*));
    errCheck(stk->data, NULL);

    return stk;
}

int8_t stackDtor(Stack* stk) {
    errCheck(stk, SE_POINTER_ERR);
    errCheck(stk->data, SE_POINTER_ERR);
    free(stk->data);
    free(stk);
    return SE_ALL_OK;
}

int8_t stackPush(Stack* stk, void* item) {
    errCheck(stk, SE_POINTER_ERR);
    errCheck(stk->data, SE_POINTER_ERR);
    if(stk->size == stk->capacity) {
        stk->capacity <<= 1;
        stkRellocate(stk);
    }
    stk->data[stk->size++] = item;
    return SE_ALL_OK;
}

int8_t stackPop(Stack* stk) {
    errCheck(stk, SE_POINTER_ERR);
    errCheck(stk->data, SE_POINTER_ERR);
    errCheck(stk->size, SE_STACK_EMPTY_ERR);

    --stk->size;

    if(
        stk->size < (stk->capacity >> 2) && 
        (stk->capacity >> 1) >= STK_START_SIZE
    ) {
        stk->capacity >>= 1;
        stkRellocate(stk);
    }
    return 0;
}

void* stackTop(Stack* stk) {
    errCheck(stk, NULL);
    errCheck(stk->data, NULL);
    errCheck(stk->size, NULL);
    return stk->data[stk->size - 1];
}

uint64_t stackSize(Stack* stk) {
    errCheck(stk, 0);
    return stk->size;
    return 0;
}