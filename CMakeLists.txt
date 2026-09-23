cmake_minimum_required(VERSION 3.16)

project(CGOL LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG 5.5
)

set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_GAMES OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(raylib)

add_executable(cgol
    src/main.cpp
)

target_link_libraries(cgol PRIVATE raylib)

if(WIN32)
    target_link_libraries(cgol PRIVATE
        winmm
        gdi32
        user32
        shell32
    )
endif()