cmake_minimum_required(VERSION 3.27)
project(polynom)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_GLIBCXX_DEBUG -D_GLIBCXX_DEBUG_PEDANTIC -fmodules-ts")
set(CMAKE_CXX_SCAN_FOR_MODULES OFF)
set(CMAKE_CXX_EXTENSIONS OFF)

set(SRCS
        ../src/single_linked_list.cpp
        ../src/monomial.cpp
        ../src/polynomial.cpp
)

add_library(${PROJECT_NAME} STATIC ${SRCS})
