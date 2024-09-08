#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <sys/time.h>

#include "GlobalHeaderTesting.h"

#define TWO_ARGUMENTS (2)

void test_generator(int32_t num_of_tests);

static inline uint64_t gettimeus(void);
static inline int32_t gen(int32_t left_border, int32_t right_border);

int main(int argc, char* argv[]) {
    assert(argc == TWO_ARGUMENTS);
    srand((uint32_t)gettimeus());
    test_generator(atoi(argv[1]));
}

void test_generator(int32_t num_of_tests) {
    int32_t heap_size = 0;
    int32_t command = 0;

    for(int32_t i = 0; i < num_of_tests; i++)
    {
        if(heap_size > 0)
            command = gen(FIB_INSERT, FIB_DEL_LAST);
        else
            command = FIB_INSERT;

        switch (command)
        {
            case FIB_INSERT: {
                printf("%d %d\n", command, gen(0, INT32_MAX - 1));
                heap_size++;
                break;
            }
            case FIB_EXT_MIN: {
                printf("%d\n", command);
                heap_size--;
                break;
            }
            case FIB_DEL_LAST: {
                printf("%d\n", command);
                heap_size--;
            }
            default: {
                break;
            }
        }
    }
}

static inline uint64_t gettimeus() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return  (uint64_t)tv.tv_sec * 1000000LL + 
            (uint64_t)tv.tv_usec;
}

static inline int32_t gen(int32_t left_border, int32_t right_border) {
    if(left_border >= right_border)
        return left_border;
    return left_border + rand() % (right_border - left_border + 1);
}
