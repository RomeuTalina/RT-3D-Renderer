cmake_minimum_required(VERSION 3.14)
set(CMAKE_CXX_COMPILER g++)
project(IdkMan CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON) 

include(FetchContent)

set(SDL_SHARED OFF CACHE BOOL "Build shared SDL library")
set(SDL_STATIC ON CACHE BOOL "Build static SDL library") 
set(SDL_TEST OFF CACHE BOOL "Build SDL test programs")

FetchContent_Declare(
    SDL3
    GIT_REPOSITORY https://github.com/libsdl-org/SDL
    GIT_TAG main
)

FetchContent_MakeAvailable(SDL3)

file(GLOB SOURCES "src/*.cpp")
add_executable(IdkMan ${SOURCES})

target_include_directories(IdkMan PRIVATE include/)
target_link_libraries(IdkMan SDL3::SDL3)
