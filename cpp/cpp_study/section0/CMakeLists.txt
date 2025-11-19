
add_executable(macro macro.cpp)
add_executable(sort sort.cpp)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_library(section0 macro.cpp)

install (TARGETS section0 DESTINATION bin)
install (FILES macro.cpp DESTINATION include)
