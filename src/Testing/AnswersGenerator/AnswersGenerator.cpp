#include <boost/heap/fibonacci_heap.hpp>

#include "../ImprovedVector/ImprovedVector.hpp"

extern "C" {
#include "../GlobalHeaderTesting.h"
}

using FibHeap =
    boost::heap::fibonacci_heap<int64_t,
                                boost::heap::compare<std::greater<int64_t>>>;
using HandleT = FibHeap::handle_type;
using HeapCitT = const boost::heap::detail::marked_heap_node<int64_t>*;
using HeapItT = boost::heap::detail::marked_heap_node<int64_t>*;
HandleT CursedCastHandle(HeapCitT it);

class Wrapper {
 public:
  Wrapper() = default;
  Wrapper(HandleT handle) : pointer_(handle.node_) {}
  bool operator==(const Wrapper& other) const {
    return pointer_ == other.pointer_;
  }
  operator HandleT() { return CursedCastHandle(pointer_); }
  HeapItT GetPointer() const { return pointer_; }

 private:
  HeapItT pointer_ = nullptr;
};

namespace std {
template <>
struct hash<Wrapper> {
  std::size_t operator()(const Wrapper& w) const noexcept {
    std::hash<size_t> h;
    return h(reinterpret_cast<size_t>(w.GetPointer()));
  }
};
}  // namespace std

void AnswerGenrator();

int main() { AnswerGenrator(); }

void AnswerGenrator(void) {
  FibHeap heap;
  ImprovedVector<Wrapper> stk;

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
        auto handle = CursedCastHandle(heap.ordered_begin().get_node());
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

HandleT CursedCastHandle(HeapCitT it) {
  return HandleT(const_cast<HeapItT>(it));
}