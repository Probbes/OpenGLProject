cmake_minimum_required(VERSION 3.10.0)
project(OpenGLProject VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if (MSVC)
    add_compile_options(/utf-8)
endif()

INCLUDE_DIRECTORIES(
    ${PROJECT_SOURCE_DIR}/vcpkg_installed/x64-windows/include
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/src/Headers
)

LINK_DIRECTORIES(
    ${PROJECT_SOURCE_DIR}/vcpkg_installed/x64-windows/lib
)

file(GLOB SOURCES
    ${PROJECT_SOURCE_DIR}/main.cpp
    ${PROJECT_SOURCE_DIR}/src/Headers*.h
    ${PROJECT_SOURCE_DIR}/src/*.cpp
)

add_executable(OpenGLProject ${SOURCES})

TARGET_LINK_LIBRARIES(OpenGLProject
    fmt glfw3dll glad assimp-vc143-mt imgui FastNoise
)