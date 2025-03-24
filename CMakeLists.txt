cmake_minimum_required(VERSION 3.10)
project(2d-render)

# Set C++ standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# Add executable
add_executable(2d-render src/main.cpp)

# Find package(s)
find_package(OpenGL REQUIRED COMPONENTS OpenGL)
find_package(GLUT REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)
#find_package(glad REQUIRED)

# Add library (optional)
add_library(glad src/glad.c)


# Add include directories
include_directories(${GLUT_INCLUDE_DIRS})
include_directories(${GLEW_INCLUDE_DIRS})
include_directories(${GLEW_INCLUDE_DIRS})
#target_include_directories(my_executable PUBLIC include)
target_include_directories(glad PUBLIC include)

# Link library to executable (optional)
target_link_libraries(2d-render OpenGL::OpenGL ${GLUT_LIBRARIES} ${GLEW_LIBRARIES} glfw glad)

# Enable testing (optional)
#enable_testing()

# Add tests (optional)
#add_test(my_test test/my_test.cpp)
#target_link_libraries(my_test my_executable)
