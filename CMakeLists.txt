cmake_minimum_required(VERSION 3.0)

project(CellularAutomaton VERSION 1.0 LANGUAGES CXX)
set(CMAKE_BUILD_TYPE Release)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
include_directories(include ${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS})

file(GLOB SOURCES source/*.cpp)

add_executable(cellular ${SOURCES})
target_link_libraries(cellular ${OPENGL_LIBRARIES} ${GLUT_LIBRARY})
