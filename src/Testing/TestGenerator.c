#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <sys/time.h>

#define TWO_ARGUMENTS (2)
enum {
    INSERT = 1,
    EXT_MIN = 2,
    DEL_LAST = 3,
};

static inline uint64_t gettimeus()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return  (uint64_t)tv.tv_sec * 1000000LL + 
            (uint64_t)tv.tv_usec;
}

int32_t gen(int32_t left_border, int32_t right_border);

int main(int argc, char* argv[]) {
    assert(argc == TWO_ARGUMENTS);
    srand((uint32_t)gettimeus());
    int32_t heap_size = 0;
    int32_t command = 0;
    for(int32_t i = 0; i < atoi(argv[1]); i++)
    {
        command = heap_size > 0 ? gen(INSERT, DEL_LAST) : INSERT;
        if(command == INSERT) {
            printf("%d %d\n", command, gen(0, INT32_MAX - 1));
            heap_size++;
        } 
        else if(command == EXT_MIN) {
            printf("%d\n", command);
            heap_size--;
        }
        else {
            printf("%d\n", command);
            heap_size--;
        }
    }
}

int32_t gen(int32_t left_border, int32_t right_border)
{
    if(left_border >= right_border)
        return left_border;
    return left_border + rand() % (right_border - left_border + 1);
}