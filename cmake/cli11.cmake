cmake_minimum_required(VERSION 3.13)
include(ExternalProject)

set(repo_cli11 "https://github.com/CLIUtils/CLI11.git")
message(STATUS "Repository for cli11: ${repo_cli11}")

ExternalProject_add(external-cli11
    PREFIX "${CMAKE_CURRENT_BINARY_DIR}/cli11"
    GIT_REPOSITORY ${repo_cli11}
    GIT_TAG c67ab9dd435ad2b9db96889e74db04016c207cf0
    GIT_SHALLOW FALSE
    GIT_SUBMODULES ""

    #PATCH_COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/patches/cli11/CMakeLists.txt" "<SOURCE_DIR>/CMakeLists.txt"

    CMAKE_ARGS
        -DCLI11_SINGLE_FILE=OFF
        -DCLI11_EXAMPLES=OFF
        -DCLI11_TESTING=OFF
        -DCMAKE_INSTALL_PREFIX=${extern_INSTALL_DIR}

    INSTALL_DIR ${extern_INSTALL_DIR}
)

add_library(cli11::cli11 INTERFACE IMPORTED)
file(MAKE_DIRECTORY ${extern_INSTALL_DIR}/include)
set_target_properties(cli11::cli11 PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${extern_INSTALL_DIR}/include)

add_dependencies(cli11::cli11 extern-cli11)
