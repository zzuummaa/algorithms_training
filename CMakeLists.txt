cmake_minimum_required(VERSION 3.10)
project(algorithms_training)

set(CMAKE_CXX_STANDARD 20)

add_compile_options(-fsanitize=address)
link_libraries(asan)

add_subdirectory(testcase)
add_subdirectory(algorithms_illuminated_part_1)
add_subdirectory(algorithms_illuminated_part_2)
add_subdirectory(mentoring)
add_subdirectory(other)
