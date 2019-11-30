cmake_minimum_required(VERSION 3.13)

project(fifo_map LANGUAGES CXX)

add_library(${PROJECT_NAME} INTERFACE)

target_include_directories(
    ${PROJECT_NAME}
    INTERFACE
    $<INSTALL_INTERFACE:include>
)

install(
    FILES src/fifo_map.hpp
    DESTINATION include
)
