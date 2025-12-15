cmake_minimum_required(VERSION 3.11...4.2)

project(RingBuffer)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

if(MSVC)
    add_compile_options(/W4 /WX)
else()
    add_compile_options(-Wall -Wextra -pedantic)
endif()

add_subdirectory(lib)

add_subdirectory(test)

include(FetchContent)

include(cmake/doxygen.cmake)
