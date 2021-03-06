cmake_minimum_required(VERSION 3.13)
include(ExternalProject)

set(repo_libxml2 "https://gitlab.gnome.org/GNOME/libxml2.git")
message(STATUS "Repository for libxml2: ${repo_libxml2}")

ExternalProject_add(extern-libxml2
    PREFIX "${CMAKE_CURRENT_BINARY_DIR}/xml2"
    GIT_REPOSITORY ${repo_libxml2}
    GIT_TAG bf2e96173d4f78f564015a925970077501586fbe 
    GIT_SHALLOW FALSE

    PATCH_COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/patches/libxml2/CMakeLists.txt" "<SOURCE_DIR>/CMakeLists.txt"
    COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/patches/libxml2/config.h.in" "<SOURCE_DIR>/config.h.in"

    CMAKE_ARGS
        -DCMAKE_INSTALL_PREFIX=${extern_INSTALL_DIR}

    INSTALL_DIR ${extern_INSTALL_DIR}
    BUILD_BYPRODUCTS ${extern_INSTALL_DIR}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}xml2${CMAKE_STATIC_LIBRARY_SUFFIX}
)

add_library(xml2::xml2 STATIC IMPORTED)
file(MAKE_DIRECTORY ${extern_INSTALL_DIR}/include)
set_target_properties(xml2::xml2 PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${extern_INSTALL_DIR}/include)
set_target_properties(xml2::xml2 PROPERTIES IMPORTED_LOCATION ${extern_INSTALL_DIR}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}xml2${CMAKE_STATIC_LIBRARY_SUFFIX})

add_dependencies(xml2::xml2 extern-libxml2)
