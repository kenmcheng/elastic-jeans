cmake_minimum_required(VERSION 3.22)

project(ElasticJeans)

set(EJ_LIB ej)
set(EJ_LIB_VERSION 0.0.1)
set(EJ_TEST ejTest)

set(gTest_tag v1.14.0)
set(openssl_tag openssl-3.2.1)

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

find_package(OpenSSL)
if (OPENSSL_FOUND)
    # Add the include directories for compiling
    target_include_directories(${EJ_LIB} PUBLIC ${OPENSSL_INCLUDE_DIR})
    # Add the static lib for linking
    target_link_libraries(${EJ_LIB} OpenSSL::SSL OpenSSL::Crypto)
    message(STATUS "Found OpenSSL ${OPENSSL_VERSION}")
else()
    message(STATUS "Downloading OpenSSL lib from github with version ${openssl_tag}")

    include(ExternalProject)
    set(OPENSSL_SOURCE_DIR ${CMAKE_CURRENT_BINARY_DIR}/_deps/openssl-src)
    set(OPENSSL_INSTALL_DIR ${CMAKE_CURRENT_BINARY_DIR}/_deps/openssl)
    set(OPENSSL_INCLUDE_DIR ${OPENSSL_INSTALL_DIR}/include)
    set(OPENSSL_CONFIGURE_COMMAND ${OPENSSL_SOURCE_DIR}/config)
    set(OPENSSL_LIBRARY_SUFFIX "a")

    ExternalProject_Add(
        OpenSSL
        SOURCE_DIR ${OPENSSL_SOURCE_DIR}
        GIT_REPOSITORY https://github.com/openssl/openssl.git
        GIT_TAG ${openssl_tag}
        USES_TERMINAL_DOWNLOAD TRUE
        CONFIGURE_COMMAND
            ${OPENSSL_CONFIGURE_COMMAND}
            --prefix=${OPENSSL_INSTALL_DIR}
            --openssldir=${OPENSSL_INSTALL_DIR}
        BUILD_COMMAND make
        TEST_COMMAND ""
        INSTALL_COMMAND make install
        INSTALL_DIR ${OPENSSL_INSTALL_DIR}
        UPDATE_COMMAND ""
    )

    file(MAKE_DIRECTORY ${OPENSSL_INCLUDE_DIR})

    add_library(OpenSSL::SSL STATIC IMPORTED GLOBAL)
    set_property(TARGET OpenSSL::SSL PROPERTY IMPORTED_LOCATION ${OPENSSL_INSTALL_DIR}/lib64/libssl.${OPENSSL_LIBRARY_SUFFIX})
    set_property(TARGET OpenSSL::SSL PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${OPENSSL_INCLUDE_DIR})
    add_dependencies(OpenSSL::SSL OpenSSL)

    add_library(OpenSSL::Crypto STATIC IMPORTED GLOBAL)
    set_property(TARGET OpenSSL::Crypto PROPERTY IMPORTED_LOCATION ${OPENSSL_INSTALL_DIR}/lib64/libcrypto.${OPENSSL_LIBRARY_SUFFIX})
    set_property(TARGET OpenSSL::Crypto PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${OPENSSL_INCLUDE_DIR})
    add_dependencies(OpenSSL::Crypto OpenSSL)

    # Add the include directories for compiling
    target_include_directories(${EJ_LIB} PUBLIC ${OPENSSL_INCLUDE_DIR})
    # Add the static lib for linking
    target_link_libraries(${EJ_LIB} OpenSSL::SSL OpenSSL::Crypto)
endif()

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