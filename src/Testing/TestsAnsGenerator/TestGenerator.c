#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <sys/time.h>

#include "../GlobalHeaderTesting.h"

/**
 * We generate randomly generate the 
 * sizes of the segments in the 
 * specified intervals, then randomly 
 * distribute the points along the segments
 */

/**
 * For INS_MIN_BORDER, the minimum value is 1
 */
#define INS_MIN_BORDER (1)
#define INS_MAX_BORDER (50)
#define EXT_MIN_BORDER (1)
#define EXT_MAX_BORDER (50)
#define DEL_MIN_BORDER (0)
#define DEL_MAX_BORDER (0)

#define NUM_MIN (-1000)
#define NUM_MAX (1000)

static inline uint64_t getTime();
void testGenerate(uint64_t test_size);

int main(int argc, char* argv[]) {
    srand((uint32_t)getTime());
    /*i.e. one argument*/
    assert(argc == 2); 
    testGenerate(strtoul(argv[1], NULL, 10));
}

static inline uint64_t getTime() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return  (uint64_t)tv.tv_sec * 1000000 + 
            (uint64_t)tv.tv_usec;
}

static inline int32_t genRand(int32_t left_border, int32_t right_border) {
    if(left_border >= right_border)
        return left_border;
    return left_border + rand() % (right_border - left_border + 1);
}

void testGenerate(uint64_t test_size) {
    const int32_t ins = genRand(INS_MIN_BORDER, INS_MAX_BORDER);
    const int32_t ext = genRand(EXT_MIN_BORDER, EXT_MAX_BORDER);
    const int32_t del = genRand(DEL_MIN_BORDER, DEL_MAX_BORDER);
    const int32_t segment_size = ins + ext + del;

    for(uint64_t current_size = 0, heap_size = 0; current_size < test_size; current_size++) {
        const int32_t point = genRand(0, segment_size);
        if(!heap_size || point <= ins) {
            printf("%d %d\n", FIB_INSERT, genRand(NUM_MIN, NUM_MAX));
            heap_size++;
        } else if(point <= ins + ext) {
            printf("%d\n", FIB_EXT_MIN);
            heap_size--;
        } else /*if(point <= ins + ext + del)*/{
            printf("%d\n", FIB_DEL_LAST);
            heap_size--;
        }
    }
}