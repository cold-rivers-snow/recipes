cmake_minimum_required(VERSION 3.10)

# 项目名称和版本
project(cpu_affinity VERSION 1.0)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)

# 查找pthread库（用于多线程）
find_package(Threads REQUIRED)

# 添加可执行文件
add_executable(thread_affinity thread_affinity.cc)
add_executable(process_affinity process_affinity.cc)
add_executable(process_thread_random_select process_thread_random_select.cc)
add_executable(process_thread_round_robin process_thread_round_robin.cc)
add_executable(process_thread_balancer_select process_thread_balancer_select.cc)

# 为每个可执行目标分别链接pthread库
target_link_libraries(thread_affinity PRIVATE Threads::Threads)
target_link_libraries(process_affinity PRIVATE Threads::Threads)
target_link_libraries(process_thread_random_select PRIVATE Threads::Threads)
target_link_libraries(process_thread_round_robin PRIVATE Threads::Threads)
target_link_libraries(process_thread_balancer_select PRIVATE Threads::Threads)

# 为每个目标添加编译选项
target_compile_options(thread_affinity PRIVATE 
    -Wall          # 启用所有警告
    -O2            # 优化级别2
)
target_compile_options(process_affinity PRIVATE 
    -Wall 
    -O2 
)
target_compile_options(process_thread_random_select PRIVATE 
    -Wall 
    -O2 
)
target_compile_options(process_thread_round_robin PRIVATE 
    -Wall 
    -O2 
)
target_compile_options(process_thread_balancer_select PRIVATE 
    -Wall 
    -O2 
)
    