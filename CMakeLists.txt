cmake_policy(SET CMP0050 NEW)

cmake_minimum_required(VERSION 3.10)

project(BFInterpreter)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(SRC_DIR ${PROJECT_SOURCE_DIR}/src)
set(INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)

if(MSVC)
    message("MSVC: Affirmative")
    add_compile_options(/W4 /O2)
else()
    message("MSVC: Negative")
    add_compile_options(-Wall -Werror -Wextra -O2)
endif()

add_executable(BFInterpreter ${SRC_DIR}/bf.cpp)

target_include_directories(BFInterpreter PUBLIC ${INCLUDE_DIR})
