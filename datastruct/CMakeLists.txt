cmake_minimum_required(VERSION 3.10)

# 项目名称和版本
project(datastruct VERSION 1.0)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)

# 添加可执行文件
add_executable(array array.cc)
add_executable(cycle_array cycle_array.cc)

# 为每个目标添加编译选项
target_compile_options(array PRIVATE 
    -Wall          # 启用所有警告
    -O2            # 优化级别2
)
target_compile_options(cycle_array PRIVATE 
    -Wall          # 启用所有警告
    -O2            # 优化级别2
)
