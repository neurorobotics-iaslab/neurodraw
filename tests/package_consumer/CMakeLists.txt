cmake_minimum_required(VERSION 3.16)
project(neurodraw_package_consumer LANGUAGES CXX)

find_package(neurodraw CONFIG REQUIRED)

add_executable(neurodraw_package_consumer main.cpp)
target_link_libraries(neurodraw_package_consumer
    PRIVATE
        neurodraw::neurodraw
        neurodraw::neurochrono
)
