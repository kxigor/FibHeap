#ifndef FIB_STACK_H
#define FIB_STACK_H

#include <assert.h>
#include <malloc.h>
#include <stdint.h>

/*!
    @defgroup stack Stack
    @brief Provides the interface for a stack data structure implementation.

    @{
 */

/** Default initial size of the stack. */
#define STK_START_SIZE (1024)

/**
 * @struct Stack
 * @brief Represents a stack data structure.
 */
typedef struct Stack {
    void** data;      /**< Pointer to the array of elements stored in the stack. */
    uint64_t size;    /**< Current number of elements in the stack. */
    uint64_t capacity; /**< Current capacity of the stack. */
} Stack;

/**
 * @enum StackErrors
 * @brief Error codes used by stack operations.
 */
typedef enum StackErrors {
    SE_ALL_OK = 0,        /**< Operation was successful. */
    SE_POINTER_ERR = 1,   /**< Error due to invalid pointer. */
    SE_MALLOC_ERR = 2,    /**< Memory allocation error. */
    SE_STACK_EMPTY_ERR = 3 /**< Error indicating the stack is empty. */
} StackErrors;

/**
 * @brief Constructs a new stack.
 * 
 * Allocates and initializes a stack with a default initial size.
 * 
 * @return A pointer to the newly created stack, or NULL if an error occurs.
 */
Stack* stackCtor();

/**
 * @brief Destructs a stack.
 * 
 * Frees the memory allocated for the stack.
 * 
 * @param stk A pointer to the stack to be destroyed.
 * 
 * @return An error code indicating success or failure.
 */
int8_t stackDtor(Stack* stk);

/**
 * @brief Pushes an element onto the stack.
 * 
 * Adds a new element to the top of the stack and increases its size.
 * 
 * @param stk A pointer to the stack.
 * @param item A pointer to the element to be pushed onto the stack.
 * 
 * @return An error code indicating success or failure.
 */
int8_t stackPush(Stack* stk, void* item);

/**
 * @brief Pops an element from the stack.
 * 
 * Removes the top element from the stack and decreases its size.
 * 
 * @param stk A pointer to the stack.
 * 
 * @return An error code indicating success or failure.
 */
int8_t stackPop(Stack* stk);

/**
 * @brief Gets the top element of the stack.
 * 
 * Retrieves the top element of the stack without removing it.
 * 
 * @param stk A pointer to the stack.
 * 
 * @return A pointer to the top element of the stack, or NULL if the stack is empty.
 */
void* stackTop(Stack* stk);

/**
 * @brief Gets the number of elements in the stack.
 * 
 * Returns the current number of elements in the stack.
 * 
 * @param stk A pointer to the stack.
 * 
 * @return The number of elements in the stack.
 */
uint64_t stackSize(Stack* stk);

/*! @} */

#endif // !FIB_STACK_H