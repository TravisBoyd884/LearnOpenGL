cmake_minimum_required(VERSION 3.16)
project(LearnOpenGL LANGUAGES C CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 23)

# --- Create your main executable ---
add_executable(Engine
    src/main.cpp
    src/glad.c         
)

# --- Include directories ---
target_include_directories(Engine PRIVATE
    ${CMAKE_SOURCE_DIR}/include    # include/ folder with glad headers
)

# --- Find system-installed packages ---
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(Threads REQUIRED)

# --- Link everything ---
target_link_libraries(Engine PRIVATE
    glfw
    OpenGL::GL
    Threads::Threads
)
