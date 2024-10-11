#ifndef FIB_GENERATOR_H
#define FIB_GENERATOR_H

#include <unistd.h>

#include <sys/stat.h>
#include <sys/types.h>

#include "../../ASM_FibHeap/ASM_FibHeap.h"
#include "../../FibHeap/FibHeap.h"
#include "../../Graph/Graph.h"
#include "../../Stack/Stack.h"
#include "../GlobalHeaderTesting.h"

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

void testingHeap(bool graphStatus);
void ASM_testingHeap(bool graphStatus);

#endif // !FIB_GENERATOR_H