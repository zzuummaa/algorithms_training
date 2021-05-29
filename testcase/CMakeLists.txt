file(GLOB CPPS "src/*.cpp")
add_library(testcase ${CPPS})
target_include_directories(testcase PUBLIC include)