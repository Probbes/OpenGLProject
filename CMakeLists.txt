cmake_minimum_required(VERSION 3.10.0)
project(OpenGLProject VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if (MSVC)
    add_compile_options(/utf-8)
endif()

INCLUDE_DIRECTORIES(
    C:/Users/Probb/Documents/GitHub/OpenGLProject/vcpkg_installed/x64-windows/include
    C:/Users/Probb/Documents/GitHub/OpenGLProject/src
    C:/Users/Probb/Documents/GitHub/OpenGLProject/src/Headers
)

LINK_DIRECTORIES(
    C:/Users/Probb/Documents/GitHub/OpenGLProject/vcpkg_installed/x64-windows/lib
)

file(GLOB SOURCES
    C:/Users/Probb/Documents/GitHub/OpenGLProject/main.cpp
    C:/Users/Probb/Documents/GitHub/OpenGLProject/src/Headers/*.h
    C:/Users/Probb/Documents/GitHub/OpenGLProject/src/*.cpp
)

add_executable(OpenGLProject ${SOURCES})

TARGET_LINK_LIBRARIES(OpenGLProject
    fmt glfw3dll glad assimp-vc143-mt imgui FastNoise
)