cmake_minimum_required(VERSION 3.16)
project(Assignment3 C)

set(CMAKE_C_STANDARD 11)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fopenmp -std=c11")


include_directories(
        ${PROJECT_SOURCE_DIR}/src
        ${MPI_INCLUDE_PATH}
)

set(PI_MPI ${PROJECT_SOURCE_DIR}/src/pi_mpi.c)
set(PI_OMP ${PROJECT_SOURCE_DIR}/src/pi_omp.c)

find_package(OpenMP REQUIRED)
find_package(MPI REQUIRED)

add_executable(pi_mpi ${PI_MPI})
add_executable(pi_omp ${PI_OMP})

if(OpenMP_CXX_FOUND)
    target_link_libraries(pi_omp PUBLIC OpenMP::OpenMP_CXX)
endif()
