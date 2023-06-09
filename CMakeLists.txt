cmake_minimum_required(VERSION 3.25)
project(pacman_game)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11")
set(CMAKE_EXE_LINKER_FLAGS "-framework Cocoa")

find_package(OpenGL REQUIRED)
find_library(GLUT_LIBRARY GLUT REQUIRED)
include_directories(include)
link_directories(${CMAKE_SOURCE_DIR}/lib)

add_executable(pacman_game main.cpp pacman.cpp pacman.h)

target_include_directories(pacman_game PRIVATE ${OPENGL_INCLUDE_DIR})
target_link_libraries(pacman_game PRIVATE ${OPENGL_LIBRARIES})
target_link_libraries(pacman_game PRIVATE ${GLUT_LIBRARY})
target_link_libraries(pacman_game PRIVATE SOIL)


