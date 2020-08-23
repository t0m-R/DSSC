cmake_minimum_required(VERSION 3.16)
project(Assignment2 C)

set(CMAKE_C_STANDARD 11)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fopenmp -std=c11")

find_package(OpenMP REQUIRED)

include_directories(
        ${PROJECT_SOURCE_DIR}/src
)

file(GLOB SOURCES
        "${PROJECT_SOURCE_DIR}/src/*.c"
        )

add_executable(assignment2 ${SOURCES})

if(OpenMP_CXX_FOUND)
    target_link_libraries(assignment2 PUBLIC OpenMP::OpenMP_CXX)
endif()
