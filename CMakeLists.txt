cmake_minimum_required(VERSION 3.2)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VERSION 0.0.${PATCH_VERSION})

project(map_reducer VERSION ${PROJECT_VERSION})

find_package(Threads)

add_executable(map_reducer main.cpp)

set_target_properties(map_reducer PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)