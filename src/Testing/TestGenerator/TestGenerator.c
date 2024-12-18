#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <sys/time.h>

#include "../GlobalHeaderTesting.h"

#define swap(a, b)            \
  do {                        \
    __typeof__(a) temp = (a); \
    a = b;                    \
    b = temp;                 \
  } while (0)

/**
 * There is a problem in the tests,
 * it must be understood that the 
 * Fibonacci heap is a multiset. Therefore, 
 * we have some freedom of choice in which 
 * element of the same is considered the 
 * minimum. Unfortunately, the implementation 
 * of the Fibonacci heap from boost differs 
 * from mine in this. Therefore: 
 * undef DIFFERENT_NUMBERS and the specified 
 * DEL_MIN/BEL_MAX segment CANNOT be ALLOWED 
 * at the SAME TIME, in this case the invariant 
 * will return an error. Here is an example 
 * when there will be a different invariant
 * gen.o:
 * 1 0
 * 1 1
 * 3
 * 1 0
 * 1 1
 * 1 1
 * 3
 * 2
 * 3
 * 2
 * Then the output will be
 * ans.o:
 * 0
 * 1
 * fib.o:
 * 0
 * 0
 * This is because in the first case,
 * the minimum is the first zero,
 * in the second - the second.
 * I don't see any point in adjusting my fibheap to boost.
 */
#define DIFFERENT_NUMBERS

#define INS_MIN_BORDER (1)
#define INS_MAX_BORDER (1000)
#define EXT_MIN_BORDER (1)
#define EXT_MAX_BORDER (50)
#define DEL_MIN_BORDER (1)
#define DEL_MAX_BORDER (50)

#define NUM_MIN (-500000)
#define NUM_MAX (500000)

void testGenerate(uint64_t test_size);

static inline time_t getTime();
static inline int32_t genRand(int32_t left_border, int32_t right_border);

int main(int argc, char* argv[]) {
  const int32_t kArhsNum = 2;
  const int32_t kBasis = 10;

  srand((uint32_t)getTime());

  assert(argc == kArhsNum);

  testGenerate(strtoul(argv[1], NULL, kBasis));
}

void testGenerate(uint64_t test_size) {
#ifdef DIFFERENT_NUMBERS
  assert(test_size <= NUM_MAX - NUM_MIN);
#endif

  const int32_t kIns = genRand(INS_MIN_BORDER, INS_MAX_BORDER);
  const int32_t kExt = genRand(EXT_MIN_BORDER, EXT_MAX_BORDER);
  const int32_t kDel = genRand(DEL_MIN_BORDER, DEL_MAX_BORDER);
  const int32_t kSegmentSize = kIns + kExt + kDel;

#ifdef DIFFERENT_NUMBERS
  int32_t* unique_arr = calloc(test_size, sizeof(int32_t));
  assert(unique_arr != NULL);
  for (size_t i = 0; i < test_size; i++) {
    unique_arr[i] = NUM_MIN + (int32_t)i;
  }
  for (size_t i = 1; i < test_size; i++) {
    size_t j = (size_t)genRand(0, (int32_t)i - 1);
    swap(unique_arr[i], unique_arr[j]);
  }
#endif

  for (size_t current_size = 0, heap_size = 0; current_size < test_size;
       current_size++) {
    const int32_t kPoint = genRand(0, kSegmentSize);
    if (!heap_size || kPoint <= kIns) {
#ifdef DIFFERENT_NUMBERS
      printf("%d %d\n", FIB_INSERT, unique_arr[current_size]);
#else
      printf("%d %d\n", FIB_INSERT, genRand(NUM_MIN, NUM_MAX));
#endif
      heap_size++;
    } else if (kPoint <= kIns + kExt) {
      printf("%d\n", FIB_EXT_MIN);
      heap_size--;
    } else /*if(kPoint <= kIns + ext + del)*/ {
      printf("%d\n", FIB_DEL_ALMOST_LAST);
      heap_size--;
    }
  }
#ifdef DIFFERENT_NUMBERS
  free(unique_arr);
#endif
}

static inline time_t getTime() {
  static const time_t kNsecPerSec = 1000000000;
  struct timeval tv = {};
  assert(gettimeofday(&tv, NULL) == 0);
  return tv.tv_sec * kNsecPerSec + tv.tv_usec;
}

static inline int32_t genRand(int32_t left_border, int32_t right_border) {
  if (left_border >= right_border) {
    return left_border;
  }
  return left_border + rand() % (right_border - left_border + 1);
}