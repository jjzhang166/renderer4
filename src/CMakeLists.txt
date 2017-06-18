cmake_minimum_required(VERSION 3.0.0)

include_directories(../include)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/build)
add_executable(renderer main.cpp glad.c Shader.cpp Shader.h Camera.cpp Camera.h Texture.h)
target_compile_features(renderer PRIVATE cxx_auto_type)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

link_directories(/usr/local/lib)
target_link_libraries (renderer glfw3 GL X11 pthread Xrandr Xi dl Xinerama Xcursor)