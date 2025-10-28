#include <iostream>
#include <cstddef>
#include <stdexcept>
#include <vector>

using namespace std;

template<typename T>
struct CycleArray {
public:

  CycleArray(size_t cap) : _size(0), _cap(cap) {
    _data.reserve(_cap);
    _start = _end = 0;
  }

  ~CycleArray() {}  
  
  size_t size() const {
    return _size;
  }

  size_t capacity() const {
    return _cap;
  }

  void push(T val) {
    if (full()) {
      _start = (_start + 1) % _cap;
    } else {
      _size++;
    }
    _data[_end] = val;
    _end = (_end + 1) % _cap;
  }

  T pop() {
    if (empty()) {
       throw std::out_of_range("CycleArray is empty. Cannot pop.");
    }

    T item = _data[_start];

    _start = (_start + 1) % _cap;

    _size--;
    return item;
  }

  const T& front() {
    if (empty()) {
      throw std::out_of_range("CycleArray is empty, cannot pop back");
    }
    return _data[_start];
  }

  void reserve(size_t cap) {
    if (cap <= _cap) {
      return;
    }
    _data.reserve(cap);
    _cap = cap;
  }

  void resize(size_t size) {
    if (size > _cap) {
      _data.resize(size);
    }
    _size = size;
  }

  bool full() const {
    return _size == _cap;
  }

  bool empty() const {
    return _size == 0;
  }

private:
  vector<T> _data;
  size_t _size;
  size_t _cap;
  size_t _start;
  size_t _end;
};

// 辅助打印函数
template <typename T>
void print_state(const CycleArray<T>& arr, const std::string& msg) {
    std::cout << "--- " << msg << " ---" << std::endl;
    std::cout << "Size: " << arr.size() << ", Full: " << (arr.full() ? "Yes" : "No") << ", Empty: " << (arr.empty() ? "Yes" : "No") << std::endl;
}

void run_test() {
    std::cout << "Starting CycleArray Test (Capacity = 4)" << std::endl;
    CycleArray<int> arr(4);
    
    // ----------------------------------------------------
    // Test 1: Basic Push and Pop (未满状态)
    // ----------------------------------------------------
    arr.push(10); // [10, -, -, -]
    arr.push(20); // [10, 20, -, -]
    print_state(arr, "After pushing 10, 20"); 
    // Output: Size: 2, Full: No, Empty: No

    std::cout << "Front element: " << arr.front() << std::endl; // Output: 10
    
    std::cout << "Popped: " << arr.pop() << std::endl; // Output: 10. Buffer conceptually: [-, 20, -, -]
    print_state(arr, "After popping 10");
    // Output: Size: 1, Full: No, Empty: No

    // ----------------------------------------------------
    // Test 2: Filling and Wrap-around (填满和环绕)
    // ----------------------------------------------------
    arr.push(30); // [-, 20, 30, -]
    arr.push(40); // [-, 20, 30, 40]
    arr.push(50); // [50, 20, 30, 40] - Now full, size 4
    print_state(arr, "After filling to max (20, 30, 40, 50)");
    // Output: Size: 4, Full: Yes, Empty: No

    // ----------------------------------------------------
    // Test 3: Overwrite (覆盖)
    // ----------------------------------------------------
    std::cout << "\nPushing 60 (should overwrite 20)" << std::endl;
    arr.push(60); // [50, 60, 30, 40] - Head moves from 20 to 30
    print_state(arr, "After pushing 60 (overwrite)"); 
    // Output: Size: 4, Full: Yes, Empty: No

    std::cout << "Popped: " << arr.pop() << std::endl; // Output: 30 (The oldest remaining element)
    std::cout << "Popped: " << arr.pop() << std::endl; // Output: 40
    std::cout << "Popped: " << arr.pop() << std::endl; // Output: 50
    print_state(arr, "After three more pops");
    // Output: Size: 1, Full: No, Empty: No

    // ----------------------------------------------------
    // Test 4: Empty and Exception Handling (清空和异常处理)
    // ----------------------------------------------------
    std::cout << "Popped: " << arr.pop() << std::endl; // Output: 60
    print_state(arr, "After final pop");
    // Output: Size: 0, Full: No, Empty: Yes
    
    // Try to pop from an empty array
    try {
        arr.pop();
    } catch (const std::out_of_range& e) {
        std::cout << "\nCaught expected exception: " << e.what() << std::endl;
    }
}

int main() {
    run_test();
    return 0;
}