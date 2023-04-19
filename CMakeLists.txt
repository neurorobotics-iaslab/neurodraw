cmake_minimum_required(VERSION 3.0)
set(CMAKE_CXX_STANDARD 11)
project(neurodraw)
set(CMAKE_BUILD_TYPE Debug)

find_package(PkgConfig)
find_library(DRAWTK_LIBS drawtk REQUIRED) 
find_path(DRAWTK_INCLUDE_DIRS drawtk.h REQUIRED) 
if(NOT DRAWTK_LIBRARIES)
  message(FATAL_ERROR "drawtk library not found")
endif()

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

include_directories(include
					${DRAWTK_INCLUDE_DIRS})

add_library(${PROJECT_NAME} 
			src/Palette.cpp
			src/Shape.cpp
			src/Rectangle.cpp
			src/Engine.cpp
			src/Display.cpp
			src/Window.cpp
			src/Rate.cpp
			src/EventHandler.cpp
			)

add_executable(window src/window.cpp)
target_link_libraries(window ${PROJECT_NAME} ${DRAWTK_LIBS} ${CMAKE_THREAD_LIBS_INIT})

