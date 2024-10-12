#include <optional>
#include <unordered_map>
#include <vector>

template<typename T>
class ImprovedVector{
 private:
 public:
  ImprovedVector() {

  };

  void push(const T& element) {
    data_.push_back(element);
    map_ind_[element] = data_.size() - 1;
  }

  void pop() {
    if (!data_.empty()) {
        map_ind_.erase(data_.back());
        data_.pop_back();
    }
  }

  std::optional<T> top() {
    if(!data_.empty()) {
      return data_.back();
    }
  }

  void erase(const T& element) {
      auto it = map_ind_.find(element);
      if (it != map_ind_.end()) {
          size_t index = it->second;
          data_[index] = data_.back();
          map_ind_[data_.back()] = index;
          data_.pop_back();
          map_ind_.erase(it);
      }
  }

 private:
  std::vector<T> data_;
  std::unordered_map<T, std::size_t> map_ind_;
};