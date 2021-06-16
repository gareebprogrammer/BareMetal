cmake_minimum_required(VERSION 3.19)
project(BareMetal)

set(CMAKE_CXX_STANDARD 11)

if(MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

find_package(OpenGL REQUIRED)
add_subdirectory(external/glad)

add_executable(BareMetal main.cpp  ${OPENGL_INCLUDE_DIRS})
target_link_libraries(BareMetal ${OPENGL_LIBRARIES} glad)
