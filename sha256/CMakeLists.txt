cmake_minimum_required(VERSION 2.6)

project(GREETWORLD)

set(CMAKE_CXX_FLAGS "-g -Wall")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build)

add_executable(sha256test sha256_test.c sha256.c)
add_executable(sha256sum sha256sum.c sha256.c)
