cmake_minimum_required(VERSION 3.16)
project(LearnOpenGL LANGUAGES C CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 23)

add_library(glad STATIC src/glad.c)
target_include_directories(glad PUBLIC
    ${CMAKE_SOURCE_DIR}/include
)
# --- Create your main executable ---
add_executable(Engine
    src/main.cpp
    src/scene.cpp
    src/stb_image.cpp
    src/texture.cpp
)

# --- Include directories ---
target_include_directories(Engine PRIVATE
    ${CMAKE_SOURCE_DIR}/include    # include/ folder with glad headers
)

target_precompile_headers(Engine PRIVATE
    ${CMAKE_SOURCE_DIR}/include/pch.hpp
)

# --- Find system-installed packages ---
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(Threads REQUIRED)

# --- Link everything ---
target_link_libraries(Engine PRIVATE
    glad
    glfw
    OpenGL::GL
    Threads::Threads
)
