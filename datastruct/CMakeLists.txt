cmake_minimum_required(VERSION 3.10)

# 项目名称和版本
project(datastruct VERSION 1.0)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)

# 添加可执行文件
add_executable(array array.cpp)
add_executable(cycle_array cycle_array.cpp)
add_executable(list list.cpp)
add_executable(double_list double_list.cpp)
add_executable(array_hash array_hash.cpp)
add_executable(bst bst.cpp)
add_executable(list_stack list_stack.cpp)
add_executable(list_queue list_queue.cpp)
add_executable(vector_stack vector_stack.cpp)
add_executable(circle_array_queue circle_array_queue.cpp)
add_executable(list_deque list_deque.cpp)
add_executable(circle_array_deque circle_array_deque.cpp)
add_executable(list_hash list_hash.cpp)
add_executable(my_hash_set hash_set_main.cpp)

# 为每个目标添加编译选项
target_compile_options(array PRIVATE 
    -Wall          # 启用所有警告
    -O2            # 优化级别2
)
# 为每个目标添加编译选项
target_compile_options(cycle_array PRIVATE 
    -Wall          # 启用所有警告
    -O2            # 优化级别2
)
# 为每个目标添加编译选项
target_compile_options(list PRIVATE 
    -Wall          # 启用所有警告
    -O2            # 优化级别2
)
# 为每个目标添加编译选项
target_compile_options(double_list PRIVATE 
    -Wall          # 启用所有警告
    -O2            # 优化级别2
)
# 为每个目标添加编译选项
target_compile_options(array_hash PRIVATE 
    -Wall          # 启用所有警告
    -O2            # 优化级别2
)
# 为每个目标添加编译选项
target_compile_options(bst PRIVATE 
    -Wall          # 启用所有警告
    -O2            # 优化级别2
)
# 为每个目标添加编译选项
target_compile_options(list_stack PRIVATE 
    -Wall          # 启用所有警告
    -O2            # 优化级别2
)
# 为每个目标添加编译选项
target_compile_options(list_queue PRIVATE 
    -Wall          # 启用所有警告
    -O2            # 优化级别2
)
# 为每个目标添加编译选项
target_compile_options(vector_stack PRIVATE 
    -Wall          # 启用所有警告
    -O2            # 优化级别2
)
# 为每个目标添加编译选项
target_compile_options(circle_array_queue PRIVATE 
    -Wall          # 启用所有警告
    -O2            # 优化级别2
)
# 为每个目标添加编译选项
target_compile_options(list_deque PRIVATE 
    -Wall          # 启用所有警告
    -O2            # 优化级别2
)
# 为每个目标添加编译选项
target_compile_options(circle_array_deque PRIVATE 
    -Wall          # 启用所有警告
    -O2            # 优化级别2
)
# 为每个目标添加编译选项
target_compile_options(list_hash PRIVATE 
    -Wall          # 启用所有警告
    -O2            # 优化级别2
)
# 为每个目标添加编译选项
target_compile_options(my_hash_set PRIVATE 
    -Wall          # 启用所有警告
    -O2            # 优化级别2
)

