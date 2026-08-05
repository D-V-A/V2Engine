cmake_minimum_required(VERSION 3.20)

project(V2Engine)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


include(FetchContent)

FetchContent_Declare(
    SDL3
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG release-3.2.20
)

FetchContent_MakeAvailable(SDL3)

add_executable(V2Engine
    src/main.cpp
)

target_link_libraries(V2Engine PRIVATE SDL3::SDL3)