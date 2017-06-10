cmake_minimum_required(VERSION 3.0.0)
project(renderer VERSION 0.0.0)

add_subdirectory(src)
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

link_directories(/usr/local/lib)