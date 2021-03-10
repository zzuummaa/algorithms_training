file(GLOB CPPS "src/*.cpp")
add_library(testcase OBJECT ${CPPS})
target_include_directories(testcase PUBLIC include)