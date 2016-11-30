cmake_minimum_required(VERSION 2.6)

project(GREETWORLD)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build)

if (CMAKE_SYSTEM_PROCESSOR MATCHES "(x86)|(X86)|(amd64)|(AMD64)")
  set (X86 TRUE)
  MESSAGE("Compiling on a x86-based system")
else (CMAKE_SYSTE_PROCESSOR MATCHES "ppc")
  set (PPC TRUE)
  MESSAGE("Compiling on a POWER-based system")
endif ()

if (X86)
  set(CMAKE_C_FLAGS "-g -Wall")
else (PPC)
  set(CMAKE_C_FLAGS "-g -Wall -maltivec -mabi=altivec")
endif ()

add_executable(sha256sum sha256sum.c sha256.c)
add_executable(research research.c)
