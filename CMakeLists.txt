cmake_minimum_required(VERSION 3.10)
project(AVL)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

if(MSVC)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /W4")
else()
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Wpedantic")
endif()

set(TEST_DIR src/AVL-tree/tests)

add_library(AVL_lib STATIC src/AVL-tree/code/AVL_tree.c)
target_include_directories(AVL_lib PUBLIC src/AVL-tree/code)

add_executable(test_AVL ${TEST_DIR}/tests.c)
target_link_libraries(test_AVL AVL_lib)
target_include_directories(test_AVL PRIVATE src/AVL-tree/code)