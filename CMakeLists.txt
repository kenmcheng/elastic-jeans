cmake_minimum_required(VERSION 3.22)

project(ElasticJeans)

set(EJ_LIB ej)
set(EJ_LIB_VERSION 0.0.1)
set(EJ_TEST ejTest)

set(gTest_tag v1.14.0)

set(CMAKE_BUILD_DIR build)
set(CMAKE_OUTPUT_DIR output)
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
    PREFIX                   ""
    SUFFIX                   ".lib.a"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_OUTPUT_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_OUTPUT_DIR}/lib"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_OUTPUT_DIR}/bin"
)

include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        ${gTest_tag}
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

file(GLOB TEST_SRCS src/test/cpp/elasticJeans/**/*.cpp src/test/cpp/elasticJeans/*.cpp)
add_executable(
    ${EJ_TEST}
    ${TEST_SRCS}
)

enable_testing()
target_link_libraries(
    ${EJ_TEST}
    GTest::gtest_main
    ${EJ_LIB}
)

include(GoogleTest)
gtest_discover_tests(${EJ_TEST})