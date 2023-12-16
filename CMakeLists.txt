cmake_minimum_required(VERSION 3.22)

project(ElasticJeans)

set(EJ_LIB ej)
set(CMAKE_BUILD_DIR build)
set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB HDRS src/main/cpp/elasticJeans/**/*.hpp src/main/cpp/elasticJeans/*.hpp)
file(GLOB SRCS src/main/cpp/elasticJeans/**/*.cpp src/main/cpp/elasticJeans/*.cpp)

add_library(${EJ_LIB} STATIC ${HDRS} ${SRCS})
target_include_directories(${EJ_LIB} PUBLIC src/main/cpp)
target_include_directories(${EJ_LIB} PUBLIC src/inc)
target_include_directories(${EJ_LIB} PUBLIC libs)

set_target_properties(${EJ_LIB}
    PROPERTIES
    PREFIX ""
    SUFFIX ".lib.a"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BUILD_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BUILD_DIR}/lib"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BUILD_DIR}/bin"
)

