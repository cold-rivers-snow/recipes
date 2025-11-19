add_executable(codestyle codestyle.cpp)
add_executable(compile_attr compile_attr.cpp)
add_executable(oop oop.cpp)
add_executable(preprocess preprocess.cpp)
add_executable(final final.cpp)

add_library(section1 codestyle.cpp compile_attr.cpp oop.cpp preprocess.cpp)

install (TARGETS section1 DESTINATION bin)
install (FILES codestyle.cpp  compile_attr.cpp oop.cpp preprocess.cpp DESTINATION include)
