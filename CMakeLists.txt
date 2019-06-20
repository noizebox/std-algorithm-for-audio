cmake_minimum_required(VERSION 3.7)
project(algorithm_perf)

set(BUFFER_SIZE 64 CACHE STRING "Audio buffer size (samples)")

set(SOURCE_FILES algorithms.cpp
			     main.cpp)

# Enumerate headers too, mostly for CLion convenience
set(HEADER_FILES algorithms.h)

add_executable(algorithm_perf ${SOURCE_FILES} ${HEADER_FILES})
target_compile_definitions(algorithm_perf PRIVATE -DBUFFER_SIZE=${BUFFER_SIZE})
target_compile_features(algorithm_perf PRIVATE cxx_std_17)
target_compile_options(algorithm_perf PRIVATE -Wall -Wextra -ffast-math -march=native)