cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(logl)
add_executable(logl src/main.cpp)

find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(logl PRIVATE glfw)

find_package(glad CONFIG REQUIRED)
target_link_libraries(logl PRIVATE glad::glad)
