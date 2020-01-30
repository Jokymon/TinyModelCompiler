cmake_minimum_required(VERSION 3.13)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

project(TinyModelCompiler
    LANGUAGES CXX C
)

set(CMAKE_CXX_STANDARD 17)

set(extern_INSTALL_DIR "${CMAKE_BINARY_DIR}/local")
include(xml2)
include(fifo_map)

add_library(libxml2_wrapper)

target_sources(libxml2_wrapper
    PRIVATE
        src/xmlpp.h
        src/xmlpp.cpp
)

target_include_directories(libxml2_wrapper
    PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
        $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/local/include>
)

target_link_libraries(libxml2_wrapper
    PRIVATE
        xml2::xml2
)

# -------------------------------------------------------------

add_executable(${PROJECT_NAME})

target_sources(${PROJECT_NAME}
    PRIVATE
        src/main.cpp
        src/opc_ua_nodeset2.h
        src/opc_ua_nodeset2.cpp
        src/modelgenerator.h
        src/modelgenerator.cpp
        src/ua_model.h
        src/ua_model.cpp
)

target_include_directories(${PROJECT_NAME}
    PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
        $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/local/include>
)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
        xml2::xml2
        fifo_map::fifo_map
        libxml2_wrapper
)

# -------------------------------------------------------------

add_executable(generate_ua_nodeset2)

target_sources(generate_ua_nodeset2
    PRIVATE
        src/generate_ua_nodeset2.cpp
        src/ua_nodeset2_generator.h
        src/ua_nodeset2_generator.cpp
        src/ua_model.h
        src/ua_model.cpp
)

target_include_directories(generate_ua_nodeset2
    PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
        $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/local/include>
)

target_link_libraries(generate_ua_nodeset2
    PUBLIC
        fifo_map::fifo_map
        libxml2_wrapper
)

