cmake_minimum_required(VERSION 3.20)

project(ElasticJeans)

set(TARGET ej.app)
set(LIB ej.lib)
set(CMAKE_CXX_STANDARD 17)
# set(cmake_cxx17_standard_compile_option "-std=c++17")
# set(cmake_cxx17_extension_compile_option "-std=gnu++17")
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB HDRS src/main/cpp/**/*.hpp src/main/cpp/*.hpp)
file(GLOB SRCS src/main/cpp/**/*.cpp src/main/cpp/*.cpp)

add_library(${LIB} ${HDRS} ${SRCS})
add_executable(${TARGET} src/main/cpp/main.cpp)
target_link_libraries(${TARGET} ${LIB})

set_target_properties(${TARGET} PROPERTIES LINKER_LANGUAGE CXX)
