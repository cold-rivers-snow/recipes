#include <iostream>
#include <vector>     // 标准库动态数组
#include <stdexcept>  // 异常处理

template <typename T>
class VectorStack {
private:
    std::vector<T> stack;  // 底层容器：std::vector（动态数组）

public:
    // 入栈：在数组尾部添加元素（栈顶）
    void push(const T& val) {
        stack.push_back(val);  // O(1) 操作（ amortized，平均复杂度）
    }

    // 出栈：移除并返回栈顶元素（数组尾部）
    T pop() {
        if (isEmpty()) {
            throw std::runtime_error("栈为空，无法出栈");
        }
        T topVal = stack.back();  // 获取尾部元素（栈顶）
        stack.pop_back();         // 移除尾部元素（O(1)）
        return topVal;
    }

    // 获取栈顶元素（不删除）
    T top() const {
        if (isEmpty()) {
            throw std::runtime_error("栈为空，无法获取栈顶元素");
        }
        return stack.back();
    }

    // 判断栈是否为空
    bool isEmpty() const {
        return stack.empty();
    }

    // 获取栈的大小
    size_t size() const {
        return stack.size();
    }

    // 打印栈元素（从栈顶到栈底）
    void print() const {
        if (isEmpty()) {
            std::cout << "栈为空" << std::endl;
            return;
        }
        std::cout << "栈顶 -> ";
        // 从尾部（栈顶）遍历到头部（栈底）
        for (auto it = stack.rbegin(); it != stack.rend(); ++it) {
            std::cout << *it << " -> ";
        }
        std::cout << "栈底" << std::endl;
    }
};

// 测试示例
int main() {
    try {
        VectorStack<int> stack;

        // 入栈操作
        stack.push(10);
        stack.push(20);
        stack.push(30);
        std::cout << "入栈后（10, 20, 30）：" << std::endl;
        stack.print();  // 输出：栈顶 -> 30 -> 20 -> 10 -> 栈底
        std::cout << "栈大小：" << stack.size() << std::endl;  // 3

        // 获取栈顶元素
        std::cout << "栈顶元素：" << stack.top() << std::endl;  // 30

        // 出栈操作
        std::cout << "出栈元素：" << stack.pop() << std::endl;  // 30
        stack.print();  // 输出：栈顶 -> 20 -> 10 -> 栈底
        std::cout << "栈大小：" << stack.size() << std::endl;  // 2

        // 清空栈
        stack.pop();  // 出栈 20
        stack.pop();  // 出栈 10
        std::cout << "清空后是否为空：" << (stack.isEmpty() ? "是" : "否") << std::endl;  // 是

        // 测试空栈操作（抛异常）
        stack.pop();  // 触发异常
    } catch (const std::exception& e) {
        std::cout << "异常：" << e.what() << std::endl;  // 输出错误信息
    }

    return 0;
}