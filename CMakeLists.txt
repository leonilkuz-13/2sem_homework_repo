cmake_minimum_required(VERSION 3.10)
project(State)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

if(MSVC)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /W4")
else()
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Wpedantic")
endif()

set(CODE_HEAP src/state/heap/code)
set(TEST_HEAP src/state/heap/tests)

add_library(Heap_lib STATIC ${CODE_HEAP}/heap.c)
target_include_directories(Heap_lib PUBLIC ${CODE_HEAP})

add_executable(test_heap ${TEST_HEAP}/tests_heap.c)
target_link_libraries(test_heap Heap_lib)
target_include_directories(test_heap PRIVATE ${CODE_HEAP})