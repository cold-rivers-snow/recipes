#include <iostream>

class Vehicle {
public:
    virtual void start() { std::cout << "Vehicle starting\n"; }
    virtual void stop() final { std::cout << "Vehicle stopping\n"; }
};

class Car final : public Vehicle {
public:
    void start() override final {  // 重写并禁止进一步重写
        std::cout << "Car starting with key\n";
    }
};

// class ElectricCar : public Car {
// public:
//     void start() override;  // ❌ 错误：Car::start() 是 final 的
// };

int main() {
    Car car;
    car.start();  // Car starting with key
    car.stop();   // Vehicle stopping
}

/*
类不能被继承	class Car final { ... };	防止继承
虚函数不能被重写	virtual void start() final;	防止 override
同时使用	void start() override final;	重写并封禁

📌 一句话记住：
final 是 C++ 中的“终结者”，用于禁止继承或禁止重写，提高代码安全性与可读性。
*/