#include <iostream>
#include <cstddef>  // 用于size_t
#include <stdexcept>  // 用于异常处理

template<typename T>
struct Array {
public:
    // 迭代器类型定义（模仿vector的迭代器行为）
    using iterator = T*;
    using const_iterator = const T*;

    // 默认构造函数：初始容量为1
    Array() : _data(new T[1]), _size(0), _cap(1) {}

    // 带初始容量的构造函数
    explicit Array(size_t cap) : _cap(cap), _size(0) {
        if (cap == 0) {
            throw std::invalid_argument("Capacity cannot be zero");
        }
        _data = new T[cap];
    }

    // 拷贝构造函数（深拷贝，避免浅拷贝问题）
    Array(const Array& other) : _size(other._size), _cap(other._cap) {
        _data = new T[_cap];
        for (size_t i = 0; i < _size; ++i) {
            _data[i] = other._data[i];
        }
    }

    // 赋值运算符重载（深拷贝）
    Array& operator=(const Array& other) {
        if (this != &other) {  // 避免自赋值
            delete[] _data;    // 释放原有资源
            _size = other._size;
            _cap = other._cap;
            _data = new T[_cap];
            for (size_t i = 0; i < _size; ++i) {
                _data[i] = other._data[i];
            }
        }
        return *this;
    }

    // 析构函数：释放动态内存
    ~Array() {
        delete[] _data;
        _data = nullptr;  // 避免野指针
    }

    // 获取元素个数
    size_t size() const { return _size; }

    // 获取容量
    size_t capacity() const { return _cap; }

    // 判断是否为空
    bool empty() const { return _size == 0; }

    // 尾插元素
    void push_back(const T& val) {
        if (_size >= _cap) {
            reserve(2 * _cap);  // 容量不足时扩容为原来的2倍
        }
        _data[_size++] = val;
    }

    // 尾删元素
    void pop_back() {
        if (empty()) {
            throw std::out_of_range("Array is empty, cannot pop back");
        }
        --_size;  // 逻辑删除（对于基础类型足够，复杂类型需调用析构）
    }

    // 访问指定索引元素（带越界检查）
    T& operator[](size_t index) {
        if (index >= _size) {
            throw std::out_of_range("Index out of range");
        }
        return _data[index];
    }

    const T& operator[](size_t index) const {
        if (index >= _size) {
            throw std::out_of_range("Index out of range");
        }
        return _data[index];
    }

    // 预留容量（仅扩容，不改变元素个数）
    void reserve(size_t new_cap) {
        if (new_cap <= _cap) return;  // 新容量小于等于当前容量则不操作
        T* new_data = new T[new_cap];
        for (size_t i = 0; i < _size; ++i) {
            new_data[i] = _data[i];
        }
        delete[] _data;
        _data = new_data;
        _cap = new_cap;
    }

    // 调整元素个数（可缩小或扩容，扩容时用默认值填充）
    void resize(size_t new_size) {
        if (new_size > _cap) {
            reserve(new_size);  // 容量不足则扩容
        }
        _size = new_size;  // 直接调整大小（缩小为逻辑删除，扩容为默认值填充）
    }

    // 插入元素到指定位置
    void insert(iterator pos, const T& val) {
        if (pos < begin() || pos > end()) {
            throw std::out_of_range("Insert position out of range");
        }
        if (_size >= _cap) {
            reserve(2 * _cap);  // 扩容
            pos = _data + (pos - begin());  // 扩容后迭代器失效，重新计算位置
        }
        // 从插入位置开始后移元素
        for (iterator it = end(); it > pos; --it) {
            *it = *(it - 1);
        }
        *pos = val;
        ++_size;
    }

    // 删除指定位置元素
    void erase(iterator pos) {
        if (pos < begin() || pos >= end()) {
            throw std::out_of_range("Erase position out of range");
        }
        // 从删除位置后一个元素开始前移
        for (iterator it = pos; it < end() - 1; ++it) {
            *it = *(it + 1);
        }
        --_size;
    }

    // 迭代器相关函数（模仿vector的迭代器接口）
    iterator begin() { return _data; }
    iterator end() { return _data + _size; }
    const_iterator begin() const { return _data; }
    const_iterator end() const { return _data + _size; }

private:
    T* _data;       // 存储元素的动态数组
    size_t _size;   // 当前元素个数（使用size_t更符合容器设计习惯）
    size_t _cap;    // 容量
};

// 测试函数
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