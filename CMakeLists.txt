cmake_minimum_required(VERSION 3.10)

# set the project name
project(BillSplitter)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# add the executable
add_executable(BillSplitter main.cpp)

# Include nlohmann json library
include_directories(${CMAKE_SOURCE_DIR}/nlohmann)

# Link the nlohmann json library
target_link_libraries(BillSplitter ${CMAKE_SOURCE_DIR}/nlohmann/json.hpp)
