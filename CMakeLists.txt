cmake_minimum_required(VERSION 3.28)

project(lgl VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS NO)

find_package(glfw3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)

add_executable(lgl
  src/main.cpp
  src/scenes/ebo_rectangle.cpp
  src/scenes/hello_triangle.cpp
)

target_link_libraries(lgl PRIVATE glfw)
target_link_libraries(lgl PRIVATE glad::glad)