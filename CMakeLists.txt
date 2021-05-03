cmake_minimum_required(VERSION 3.10)
project(CHIP-8)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/")

set(CMAKE_PREFIX_PATH)

add_executable(${PROJECT_NAME} src/main.cpp)

find_package(SDL2 REQUIRED)

include_directories(${PROJECT_NAME} ${SDL2_INCLUDE_DIR})

target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARY})      