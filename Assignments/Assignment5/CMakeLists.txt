cmake_minimum_required(VERSION 3.16)
project(Assignment5 C)

set(CMAKE_C_STANDARD 11)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11")

include_directories(
        ${PROJECT_SOURCE_DIR}/src
        ${MPI_INCLUDE_PATH}
)

find_package(MPI REQUIRED)

file(GLOB SOURCES
        "${PROJECT_SOURCE_DIR}/src/*.c"
        )

add_executable(Assignment5 ${SOURCES})
