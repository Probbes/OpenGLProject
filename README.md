# OpenGL Project

## Introduction
This is an OpenGL Project made in C++ for learning purposes. The goal is to make a game.

## What's inside
A working demo of what I'm currently doing :
- Moving the player and the camera with keyboard and mouse
- Procedural generation of terrain
- 3D assets importer with textures baked
- Basic Lights

## How to Build
This project is also teaching me about version control, building cpp projects and managing libraries.
It uses VCPKG and CMake.
VCPKG needs to be downloaded via its github repo : https://learn.microsoft.com/en-us/vcpkg/get_started/get-started-packaging?pivots=shell-powershell.
Learn how to use VCPKG here : https://learn.microsoft.com/en-us/vcpkg/get_started/get-started-packaging?pivots=shell-powershell.
By typing "vcpkg install", it should install all dependencies from the manifest "vcpkg.json" inside the folder /vcpkg_install.

The project includes FastNoise, which is not referenced in VCPKG. You can download it here : ?

Build then with CMake.