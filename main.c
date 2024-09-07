#include <stdio.h>

#include "./src/FibHeap/FibHeap.h"
#include "./src/Graph/Graph.h"

typedef enum Commands {
    INSERT = 1,
    EXT_MIN = 2,
    DEL_LAST = 3
} Commands;

void testing();

/*
    Necessary for testing deletion (in particular key reduction)
    It is worth taking the TestingStack structure as a plug, 
    so I did not do any special checks
*/
#define DEFAULT_TS_STACK_SIZE (1000)

typedef struct TestingStack {
    FibNode** data;
    uint64_t size;
    uint64_t capacity;
} TestingStack;

TestingStack*   TSctor(uint64_t);
void            TSpush(TestingStack*, FibNode*);
FibNode*        TSback(TestingStack*);
void            TSdele(TestingStack*, FibNode*);
void            TSdtor(TestingStack*);

int32_t main(void) {
    testing();
}

void testing() {
    FibHeap* heap = fibHeapCtor();
    TestingStack* stk = TSctor(DEFAULT_TS_STACK_SIZE);

    int32_t command = 0;
    while(scanf("%d", &command) == 1) {
        switch (command)
        {
            case INSERT: {
                int64_t number = 0;
                scanf("%ld", &number);
                TSpush(stk, fibHeapIns(heap, number));
                break;
            }
            case EXT_MIN: {
                printf("%ld\n", fibHeapGetMin(heap)->key);
                TSdele(stk, fibHeapGetMin(heap));
                fibHeapExtMin(heap);
                break;
            }
            case DEL_LAST: {
                fibHeapDel(heap, TSback(stk));
                break;
            }
            default:
                break;
        }
        //generateFibHeapDot(heap);
    }
    TSdtor(stk);
    fibHeapDtor(heap);
}

TestingStack* TSctor(uint64_t capacity) {
    TestingStack* stk = malloc(sizeof(TestingStack));
    assert(stk != NULL);
    stk->data = calloc(capacity, sizeof(FibNode*));
    assert(stk->data != NULL);
    stk->capacity = capacity;
    stk->size = 0;
    return stk;
}

void TSpush(TestingStack* stk, FibNode* node) {
    assert(stk != NULL);
    assert(node != NULL);
    if(stk->size >= stk->capacity) {
        stk->capacity <<= 1;
        stk->data = realloc(stk->data, sizeof(FibNode*) * stk->capacity);
        assert(stk->data != NULL);
    }
    stk->data[stk->size++] = node;
}

FibNode* TSback(TestingStack* stk) {
    assert(stk != NULL);
    assert(stk->size > 0);
    return stk->data[--stk->size];
}

/*shit code*/
void TSdele(TestingStack* stk, FibNode* node) {
    assert(stk != NULL);
    assert(node != NULL);
    for(uint64_t pos = 0; pos < stk->size; pos++) {
        if(stk->data[pos] == node) {
            for(uint64_t new_pos = pos; new_pos < stk->size - 1; new_pos++)
                stk->data[new_pos] = stk->data[new_pos + 1];
            stk->size--;
            break;
        }
    }
}

void TSdtor(TestingStack* stk) {
    assert(stk != NULL);
    assert(stk->data != NULL);
    free(stk->data);
    free(stk);
}