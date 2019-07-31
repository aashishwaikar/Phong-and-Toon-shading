cmake_minimum_required(VERSION 3.8)

if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  set(CMAKE_TOOLCHAIN_FILE "C:\\vcpkg\\scripts\\buildsystems\\vcpkg.cmake" CACHE PATH "toolchain file")
endif()

project(CS380)

find_package(OpenGL REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(GLEW REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(Assimp REQUIRED)

add_subdirectory(Engine)
add_subdirectory(Application)