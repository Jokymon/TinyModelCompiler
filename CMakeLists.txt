cmake_minimum_required(VERSION 3.13)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

project(TinyModelCompiler
    LANGUAGES CXX C
)

include(ExternalProject)

set(CMAKE_CXX_STANDARD 14)

set(extern_INSTALL_DIR "${CMAKE_BINARY_DIR}/local")
include(xml2)

add_executable(${PROJECT_NAME})

target_sources(${PROJECT_NAME}
    PRIVATE
        src/xmlpp.cpp
        src/main.cpp
        src/modelgenerator.cpp
)

target_include_directories(${PROJECT_NAME}
    PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
        $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/local/include>
)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
        xml2
)
