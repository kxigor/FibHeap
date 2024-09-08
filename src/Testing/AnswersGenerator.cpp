#include <algorithm>
#include <iostream>
#include <queue>
#include <vector>

#include "GlobalHeaderTesting.h"

/*
    Let's rather lazily emulate the 
    actions of the Fibonacci heap
*/
void answer_genrator(void);

int main(void) {
    answer_genrator();
}

void answer_genrator(void) {
    std::vector<int64_t>vec;
    std::vector<int64_t>::iterator it;

    int32_t command = 0;
    int64_t number = 0;
    while(std::cin >> command) {
        switch (command)
        {
            case FIB_INSERT: {
                std::cin >> number;
                vec.push_back(number);
                break;
            }
            case FIB_EXT_MIN: {
                it = std::min_element(vec.begin(), vec.end());
                std::cout << *it << '\n';
                vec.erase(it);
                break;
            }
            case FIB_DEL_LAST: {
                vec.pop_back();
            }
            default: {
                break;
            }
        }
    }
}