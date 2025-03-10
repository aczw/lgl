cmake_minimum_required(VERSION 3.28..3.31)

project(lgl VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS NO)

find_package(glfw3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(Stb REQUIRED)
find_package(glm CONFIG REQUIRED)

set(SRC_DIR "src")
set(SCENES_DIR "${SRC_DIR}/scenes")
set(GETTING_STARTED_DIR "${SCENES_DIR}/getting_started")

add_executable(lgl
  ${SRC_DIR}/main.cpp
  ${SRC_DIR}/shaderprogram.cpp
  ${SRC_DIR}/util.cpp
  ${SRC_DIR}/stb_image.cpp

  ${GETTING_STARTED_DIR}/hello_triangle/hello_triangle.cpp
  ${GETTING_STARTED_DIR}/shaders/shaders.cpp
  ${GETTING_STARTED_DIR}/textures/textures.cpp
  ${GETTING_STARTED_DIR}/transformations/transformations.cpp
)

target_link_libraries(lgl PRIVATE glfw)
target_link_libraries(lgl PRIVATE glad::glad)
target_include_directories(lgl PRIVATE ${Stb_INCLUDE_DIR})
target_link_libraries(lgl PRIVATE glm::glm)