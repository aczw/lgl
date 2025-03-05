cmake_minimum_required(VERSION 3.28)

project(lgl VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS NO)

find_package(glfw3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)

set(SRC_DIR "src")
set(SCENES_DIR "${SRC_DIR}/scenes")

add_executable(lgl
  ${SRC_DIR}/main.cpp
  ${SRC_DIR}/shaderprogram.cpp

  ${SCENES_DIR}/ebo_rectangle/ebo_rectangle.cpp
  ${SCENES_DIR}/hello_triangle/hello_triangle.cpp
  ${SCENES_DIR}/intro_shaders/intro_shaders.cpp
)

target_link_libraries(lgl PRIVATE glfw)
target_link_libraries(lgl PRIVATE glad::glad)