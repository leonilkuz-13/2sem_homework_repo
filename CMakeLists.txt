cmake_minimum_required(VERSION 3.10)
project(CSV)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

if(MSVC)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /W4")
else()
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Wpedantic")
endif()

set(SOURCES
    src/CSV/struct.c
    src/CSV/parser.c
    src/CSV/tools.c
    src/CSV/graphics.c
    src/CSV/main.c
)

add_executable(csv_parser ${SOURCES})
target_include_directories(csv_parser PRIVATE src/CSV)