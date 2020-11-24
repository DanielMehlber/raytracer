#        __            _      __               __    ____             
#   ____/ /___ _____  (_)__  / /___ ___  ___  / /_  / / /_  ___  _____
#  / __  / __ `/ __ \/ / _ \/ / __ `__ \/ _ \/ __ \/ / __ \/ _ \/ ___/
# / /_/ / /_/ / / / / /  __/ / / / / / /  __/ / / / / /_/ /  __/ /    
# \__,_/\__,_/_/ /_/_/\___/_/_/ /_/ /_/\___/_/ /_/_/_.___/\___/_/     
#  https://github.com/danielmehlber                                     


cmake_minimum_required(VERSION 3.0)

project(raytracer)

# Not necessary on windows, just on linux
find_package (Threads REQUIRED)


add_executable(
                raytracer 
                main.cpp 
                raytracer.cpp
                processing.cpp
                timing.cpp
            )

target_link_libraries(raytracer
    ${CMAKE_THREAD_LIBS_INIT}
)



