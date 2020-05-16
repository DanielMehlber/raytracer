cmake_minimum_required(VERSION 3.0)

project(raytracer)

add_executable(raytracer main.cpp raytracer.cpp)


add_subdirectory(SDL-mirror)

message("CMake: SDL2")
target_include_directories(raytracer PUBLIC ${SDL2_INCLUDE_DIR})
target_link_libraries(raytracer PUBLIC ${SDL2_LIBRARY})