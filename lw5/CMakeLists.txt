project(lw5)
set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE SRC "*.h" "*.cpp")
add_executable(lw5 ${SRC})
SET(CMAKE_BINARY_DIR ./)