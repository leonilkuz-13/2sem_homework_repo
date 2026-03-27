cmake_minimum_required(VERSION 3.10)
project(CSV)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

if(MSVC)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /W4")
else()
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Wpedantic")
endif()

set(CODE_DIR src/CSV/code)
set(TEST_DIR src/CSV/test)

set(LIB_SOURCES
    ${CODE_DIR}/struct.c
    ${CODE_DIR}/parser.c
    ${CODE_DIR}/tools.c
    ${CODE_DIR}/graphics.c
)

add_library(csv_lib STATIC ${LIB_SOURCES})
target_include_directories(csv_lib PUBLIC ${CODE_DIR})

add_executable(csv_parser ${CODE_DIR}/main.c)
target_link_libraries(csv_parser csv_lib)
target_include_directories(csv_parser PRIVATE ${CODE_DIR})

add_executable(test_struct ${TEST_DIR}/test_struct.c)
target_link_libraries(test_struct csv_lib)
target_include_directories(test_struct PRIVATE ${CODE_DIR})

add_executable(test_parser ${TEST_DIR}/test_parser.c)
target_link_libraries(test_parser csv_lib)
target_include_directories(test_parser PRIVATE ${CODE_DIR})