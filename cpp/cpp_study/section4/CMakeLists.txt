# === 新增：查找 pybind11 ===
find_package(pybind11 REQUIRED)
find_package(Python3 COMPONENTS Interpreter Development REQUIRED)
find_package(pybind11 CONFIG REQUIRED)
# 查找 Protobuf（需要 libprotobuf-dev 和 protobuf-compiler）
find_package(Protobuf REQUIRED)
# 查找 gperftools 的 libprofiler
find_library(PROFILER_LIBRARY
    NAMES profiler
    DOC "Path to libprofiler.so"
)
if(NOT PROFILER_LIBRARY)
    message(FATAL_ERROR "libprofiler not found. Please install libgoogle-perftools-dev (Ubuntu/Debian) or gperftools-devel (CentOS/RHEL)")
endif()

# ==================== Protobuf 代码生成 ====================

set(PROTO_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/sample.proto
    # ${CMAKE_CURRENT_SOURCE_DIR}/sample_v3.proto
)

set(EXISTING_PROTO_FILES)
foreach(proto_file IN LISTS PROTO_FILES)
    if(EXISTS ${proto_file})
        list(APPEND EXISTING_PROTO_FILES ${proto_file})
    endif()
endforeach()

if(EXISTING_PROTO_FILES)
    protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS ${EXISTING_PROTO_FILES})
else()
    message(WARNING "No .proto files found. Skipping protobuf code generation.")
    set(PROTO_SRCS)
    set(PROTO_HDRS)
endif()

# 添加头文件-only 的 INTERFACE 库：common
add_library(section4_common INTERFACE)

target_include_directories(section4_common
        INTERFACE
        ${PROTOBUF_INCLUDE_DIRS}
        ${Python3_INCLUDE_DIRS}
)

set(SOURCES
    curl.cpp
    gperf.cpp
    json.cpp
    lua_shared.cpp
    luajit.cpp
    msgpack.cpp
    protobuf.cpp
    pybind.cpp
    zmq.cpp
    cpr.cpp
)


list(APPEND SOURCES ${PROTO_SRCS})

# ==================== 可执行文件 ====================

add_executable(cpr_test cpr.cpp)
target_link_libraries(cpr_test common section4_common ${CPR_LIBRARY} ${CURL_LIBRARY} pthread)

add_executable(curl_test curl.cpp)
target_link_libraries(curl_test common section4_common ${CURL_LIBRARY})

add_executable(gperf gperf.cpp)
target_link_libraries(gperf common section4_common ${PROFILER_LIBRARY})

add_executable(json json.cpp)
target_link_libraries(json common)

add_executable(luajit luajit.cpp)
target_link_libraries(luajit common ${LUAJIT_LIBRARY})

add_executable(msgpack msgpack.cpp)
target_link_libraries(msgpack common section4_common)

add_executable(protobuf protobuf.cpp)
target_link_libraries(protobuf common section4_common protobuf::libprotobuf)

pybind11_add_module(pydemo pybind.cpp)

add_executable(zmq_test zmq.cpp)
target_link_libraries(zmq_test common zmq pthread)

# ====== 共享库：lua_shared ======
add_library(lua_shared SHARED lua_shared.cpp)
target_link_libraries(lua_shared common)
if(Lua_FOUND)
    target_link_libraries(lua_shared ${LUA_LIBRARIES})
    target_include_directories(lua_shared PRIVATE ${LUA_INCLUDE_DIR})
endif()
set_target_properties(lua_shared PROPERTIES
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED ON
    OUTPUT_NAME "lua_shared"
)

# ==================== 静态库（不包含 cpr.cpp）====================

add_library(section4 STATIC ${SOURCES})
target_link_libraries(section4
    common
    section4_common
    protobuf::libprotobuf
)

# ==================== 安装规则 ====================

install(TARGETS
    cpr_test curl_test gperf json luajit msgpack protobuf zmq_test
    RUNTIME DESTINATION bin
)

install(TARGETS section4
    lua_shared
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
)

install(FILES
    ${COMMON_DIR}/json.hpp
    ${PROTO_HDRS}
    DESTINATION include/common
)

set(RESOURCES
    demo.py
    embedded.lua
    flame.svg
    icicle.svg
    sample.proto
    sample_v3.proto
    setup.sh
    shared.lua
)

install(FILES
    ${RESOURCES}
    DESTINATION share/${PROJECT_NAME}
)

# ==================== 输出提示 ====================

message(STATUS "Project: ${PROJECT_NAME}")
message(STATUS "Protobuf include: ${PROTOBUF_INCLUDE_DIRS}")
message(STATUS "Protobuf libs: ${PROTOBUF_LIBRARIES}")
message(STATUS "Using libprofiler: ${PROFILER_LIBRARY}")