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

SET (CMAKE_C_COMPILER "xlc")

IF (x86)
  IF (CMAKE_C_COMPILER STREQUAL "gcc")
    set(CMAKE_C_FLAGS "-g -Wall")
  ENDIF ()
ELSE (ppc)
  IF(CMAKE_C_OMPILER_ID STREQUAL "gcc")
    SET(CMAKE_C_FLAGS "-g -Wall -maltivec -mabi=altivec")
  ELSE ()
    SET(CMAKE_C_FLAGS "-g -qaltivec -qarch=auto -O4")
  ENDIF ()
ENDIF ()

ADD_EXECUTABLE(sha256sum sha256sum.c sha256.c)

IF (PPC)
  ADD_EXECUTABLE(vsha256sum vsha256sum.c vsha256.c)
ENDIF()

