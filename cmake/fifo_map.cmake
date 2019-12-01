set(repo_fifo_map "https://github.com/nlohmann/fifo_map.git")
message(STATUS "Repository for fifo_map: ${repo_fifo_map}")

ExternalProject_add(external-fifo_map
    PREFIX "${CMAKE_CURRENT_BINARY_DIR}/fifo_map"
    GIT_REPOSITORY ${repo_fifo_map}
    GIT_TAG a5a5b4dc92b0031ec30b37b6c29944d450054df5
    GIT_SHALLOW FALSE

    PATCH_COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/patches/fifo_map/CMakeLists.txt" "<SOURCE_DIR>/CMakeLists.txt"

    CMAKE_ARGS
        -DCMAKE_INSTALL_PREFIX=${extern_INSTALL_DIR}

    INSTALL_DIR ${extern_INSTALL_DIR}
)

add_library(fifo_map INTERFACE IMPORTED)
add_dependencies(fifo_map extern-fifo_map)