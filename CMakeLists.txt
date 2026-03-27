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
set(CODE_GRAPH src/state/graph/code)
set(TEST_GRAPH src/state/graph/tests)


add_library(Heap_lib STATIC ${CODE_HEAP}/heap.c)
target_include_directories(Heap_lib PUBLIC ${CODE_HEAP})

add_executable(test_heap ${TEST_HEAP}/tests_heap.c)
target_link_libraries(test_heap Heap_lib)
target_include_directories(test_heap PRIVATE ${CODE_HEAP})

add_library(Graph_lib STATIC ${CODE_GRAPH}/graph.c)
target_include_directories(Graph_lib PUBLIC ${CODE_GRAPH})

add_executable(test_graph ${TEST_GRAPH}/tests_graph.c)
target_link_libraries(test_graph Graph_lib)
target_include_directories(test_graph PRIVATE ${CODE_GRAPH})

add_executable(state src/state/state.c)
target_link_libraries(state Heap_lib Graph_lib)
target_include_directories(state PRIVATE ${CODE_HEAP} ${CODE_GRAPH})

enable_testing()
set(TEST_DATA_DIR "${CMAKE_SOURCE_DIR}/src/state")
add_test(NAME TestBase      COMMAND state "${TEST_DATA_DIR}/test1.txt")
add_test(NAME TestLine      COMMAND state "${TEST_DATA_DIR}/test2.txt")
add_test(NAME TestStar      COMMAND state "${TEST_DATA_DIR}/test3.txt")
