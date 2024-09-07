#include <algorithm>
#include <iostream>
#include <queue>
#include <vector>

int main(void) {
    std::priority_queue<int64_t, std::vector<int64_t>, std::greater<int64_t>> q;
    std::vector<int64_t>vec;

    int32_t command = 0;
    while(std::cin >> command) {
        switch (command)
        {
            case 1: {
                int64_t number = 0;
                std::cin >> number;
                vec.push_back(number);
                break;
            }
            case 2: {
                auto m = std::min_element(vec.begin(), vec.end());
                std::cout << *m << '\n';
                vec.erase(m);
                break;
            }
            case 3: {
                vec.pop_back();
            }
            default:
                break;
        }
    }
}