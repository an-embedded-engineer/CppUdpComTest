# Set CMake Version
cmake_minimum_required(VERSION 3.1)

# Set Project Name
project(CppUdpComTest CXX)

# Set C++ Options
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "-Wall")

# Add Packages
find_package(Threads REQUIRED)

# Add Sub Directories
add_subdirectory(Lib)
add_subdirectory(Receiver)
add_subdirectory(Sender)
