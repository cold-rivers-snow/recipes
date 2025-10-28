#include <iostream>
#include <cstddef>
#include <stdexcept>

template<typename T>
struct Array {
public:
  using iterator = T*;
  using const_iterator = const T*;

  Array() : _size(0), _cap(1) {
    _data = new T[_cap];
  }

  explicit Array(size_t cap) : _size(0), _cap(cap) {
    if (cap < 1) {
      throw std::invalid_argument("Capacity must greate zero");
    }
    _data = new T[_cap];
  }

  Array(const Array& other) : _size(other._size), _cap(other._cap) {
    _data = new T[_cap];
    for (size_t i = 0; i < other.size(); i++) {
      _data[i] = other._data[i];
    }
  }

  Array operator=(const Array& other) {
    if (this != &other) {
      delete []_data;
      _size = other._size;
      _cap = other._cap;
      _data = new T[other._data];
      for (size_t i = 0; i < _size; i++) {
        _data[i] = other._data[i];
      }
    }
  }

  ~Array() {
    delete []_data;
    _data = nullptr;
  }

  size_t size() const {
    return _size;
  }

  size_t capacity() const {
    return _cap;
  }

  void push_back(T val) {
    if (_size >= _cap) {
      reserve(2 * _cap);
    }
    _data[_size++] = val;
  }

  T& operator[](size_t index) {
    if (index >= _size) {
      throw std::out_of_range("Index out of range.");
    }
    return _data[index];
  }

  const T& operator[](size_t index) const {
    if (index >= _size) {
      throw std::out_of_range("Index out of range.");
    }
    return _data[index];
  }

  T pop_back() {
    if (empty()) {
      throw std::out_of_range("Array is empty, cannot pop back");
    }
    return _data[_size--];
  }

  void insert(iterator pos, const T& val) {
    if (pos < begin() || pos > end()) {
      throw std::out_of_range("Insert position out of range");
    }
    if (_size >= _cap) {
      reserve(2 * _cap);
      pos = _data + (pos - begin());
    }
    for (iterator it = end(); it > pos; --it) {
      *it = *(it - 1);
    }
    *pos = val;
    ++_size;
  }

  void erase(iterator pos) {
    if (pos < begin() || pos >= end()) {
      throw std::out_of_range("Erase position out of range");
    }
    for (iterator it = pos; it < end() - 1; ++it) {
      *it = *(it + 1);
    }
    --_size;
  }

  void reserve(size_t cap) {
    if (cap <= _cap) {
      return;
    }
    T* new_data = new T[cap];
    _cap = cap;
    for (size_t i = 0; i < size(); i++) {
      new_data[i] = _data[i];
    }
    delete [] _data;
    _data = new_data;
  }

  void resize(size_t size) {
    if (size > _cap) {
      reserve(size);
    }
    _size = size;
  }

  iterator begin() {
    return _data;
  }

  iterator end() {
    return _data + _size;
  }

  const_iterator begin() const {
    return _data;
  }

  const_iterator end() const {
    return _data + _size;
  }

  bool empty() {
    return _size == 0;
  }

private:
  T* _data;
  size_t _size;
  size_t _cap;
};

int main() {
  try {
    // 测试构造和push_back
    Array<int> arr;
    arr.push_back(10);
    arr.push_back(20);
    arr.push_back(30);
    std::cout << "Size: " << arr.size() << ", Capacity: " << arr.capacity() << std::endl;  // 3,4

    // 测试访问元素
    for (size_t i = 0; i < arr.size(); ++i) {
        std::cout << arr[i] << " ";  // 10 20 30
    }
    std::cout << std::endl;

    // 测试迭代器
    for (auto it = arr.begin(); it != arr.end(); ++it) {
        std::cout << *it << " ";  // 10 20 30
    }
    std::cout << std::endl;

    // 测试插入
    arr.insert(arr.begin() + 1, 15);
    std::cout << "After insert: ";
    for (int val : arr) {  // 范围for循环依赖迭代器
        std::cout << val << " ";  // 10 15 20 30
    }
    std::cout << std::endl;

    // 测试删除
    arr.erase(arr.begin() + 2);
    std::cout << "After erase: ";
    for (int val : arr) {
        std::cout << val << " ";  // 10 15 30
    }
    std::cout << std::endl;

    // 测试拷贝构造
    Array<int> arr2 = arr;
    arr2.push_back(40);
    std::cout << "arr2: ";
    for (int val : arr2) {
        std::cout << val << " ";  // 10 15 30 40
    }
    std::cout << std::endl;
  } catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << std::endl;
  }

  return 0;
}