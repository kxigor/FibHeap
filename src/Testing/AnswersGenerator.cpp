#include <boost/heap/fibonacci_heap.hpp>

#include "./ImprovedVector.hpp"

extern "C" {
#include "./GlobalHeaderTesting.h"
}

void answer_genrator(void);

int main(void) { answer_genrator(); }

using fibheap =
    boost::heap::fibonacci_heap<int64_t,
                                boost::heap::compare<std::greater<int64_t>>>;

class wrapper {
 public:
  wrapper() = default;
  wrapper(fibheap::handle_type handle) : handle_(handle.node_) {}
  bool operator==(const wrapper& other) const {
    return handle_ == other.handle_;
  }
  operator fibheap::handle_type() {
    return fibheap::handle_type(
        const_cast<boost::heap::detail::marked_heap_node<int64_t>*>(handle_));
  }
  boost::heap::detail::marked_heap_node<int64_t>* handle_ = nullptr;
};

namespace std {
template <>
struct hash<wrapper> {
  std::size_t operator()(const wrapper& w) const noexcept {
    std::hash<size_t> h;
    return h(reinterpret_cast<size_t>(w.handle_));
  }
};
}  // namespace std

void answer_genrator(void) {
  fibheap heap;
  ImprovedVector<wrapper> stk;

  int32_t command = 0;
  while (scanf("%d", &command) == 1) {
    switch (command) {
      case FIB_INSERT: {
        int64_t number = 0;
        scanf("%ld", &number);
        stk.push(heap.push(number));
        break;
      }
      case FIB_EXT_MIN: {
        printf("%ld\n", heap.top());
        auto handle = fibheap::handle_type(
            const_cast<boost::heap::detail::marked_heap_node<int64_t>*>(
                heap.ordered_begin().get_node()));
        stk.erase(handle);
        heap.pop();
        break;
      }
      case FIB_DEL_ALMOST_LAST: {
        heap.erase(stk.top().value());
        stk.pop();
        break;
      }
      default:
        break;
    }
  }
}