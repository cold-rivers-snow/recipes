cmake_minimum_required(VERSION 3.14)
project(cpp_study)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories("${PROJECT_BINARY_DIR}")
include_directories(${CMAKE_SOURCE_DIR}/common)

# 设置 common 和资源目录
set(COMMON_DIR "${CMAKE_SOURCE_DIR}/common")
set(RESOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}")

# === 1. 查找 LuaJIT ===
# 查找头文件
find_path(LUAJIT_INCLUDE_DIR luajit.h
    PATHS /usr/local/include/luajit-2.1 /usr/include/luajit-2.1
    NO_DEFAULT_PATH
)

# 查找库文件
find_library(LUAJIT_LIBRARY NAMES luajit-5.1 luajit
    PATHS /usr/local/lib /usr/lib
    NO_DEFAULT_PATH
)

if(NOT LUAJIT_INCLUDE_DIR OR NOT LUAJIT_LIBRARY)
    message(FATAL_ERROR "LuaJIT not found! Please install libluajit-5.1-dev")
endif()

if(NOT LUAJIT_INCLUDE_DIR)
    message(FATAL_ERROR "LuaJIT header (luajit.h) not found!")
endif()

if(NOT LUAJIT_LIBRARY)
    message(FATAL_ERROR "LuaJIT library (libluajit-5.1.so) not found!")
endif()

find_path(LUABRIDGE_INCLUDE_DIR LuaBridge/LuaBridge.h
    PATHS
        ${CMAKE_SOURCE_DIR}/common/LuaBridge-2.6/Source
        /usr/local/include
        /opt/include
    NO_DEFAULT_PATH  # 可选：只在指定路径中找
)

if(NOT LUABRIDGE_INCLUDE_DIR)
    message(FATAL_ERROR "LuaBridge header (LuaBridge.h) not found!")
endif()

# 查找 libcurl
find_library(ZMQ_LIBRARY zmq REQUIRED)
find_library(CPR_LIBRARY cpr REQUIRED)
find_library(CURL_LIBRARY curl REQUIRED)

# 添加头文件-only 的 INTERFACE 库：common
add_library(common INTERFACE)

# 设置 common 接口包含路径
target_include_directories(common
    INTERFACE
        ${COMMON_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${LUAJIT_INCLUDE_DIR}
        ${LUABRIDGE_INCLUDE_DIR}
)

# 设置 C++11 标准
target_compile_features(common INTERFACE cxx_std_11)

include_directories("${PROJECT_SOURCE_DIR}/section0")
include_directories("${PROJECT_SOURCE_DIR}/section1")
include_directories("${PROJECT_SOURCE_DIR}/section2")
include_directories("${PROJECT_SOURCE_DIR}/section3")
include_directories("${PROJECT_SOURCE_DIR}/section4")
include_directories("${PROJECT_SOURCE_DIR}/section5")
add_subdirectory(section0)
add_subdirectory(section1)
add_subdirectory(section2)
add_subdirectory(section3)
add_subdirectory(section4)
add_subdirectory(section5)
set(EXTRA_LIBS ${EXTRA_LIBS} section0)
set(EXTRA_LIBS ${EXTRA_LIBS} section1)
set(EXTRA_LIBS ${EXTRA_LIBS} section2)
set(EXTRA_LIBS ${EXTRA_LIBS} section3)
set(EXTRA_LIBS ${EXTRA_LIBS} section4)
set(EXTRA_LIBS ${EXTRA_LIBS} section5)
