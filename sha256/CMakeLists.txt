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

IF (x86)
  set(CMAKE_C_FLAGS "-g -Wall")
ELSE (ppc)
  set(CMAKE_C_FLAGS "-g -Wall -maltivec -mabi=altivec")
ENDIF ()

ADD_EXECUTABLE(sha256sum sha256sum.c sha256.c)
ADD_EXECUTABLE(research research.c)

IF (PPC)
  ADD_EXECUTABLE(vsha256sum sha256sum.c sha256.c)
  SET_TARGET_PROPERTIES(vsha256sum PROPERTIES COMPILE_DEFINITIONS "VECTORIZED=TRUE")
ENDIF()

