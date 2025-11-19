cmake_minimum_required(VERSION 3.10)

# 项目名称和版本
project(design_pattern VERSION 1.0)

# 设置C++标准
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)


# 添加可执行文件
add_executable(observer observer.cpp)
