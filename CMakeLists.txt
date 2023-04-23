cmake_minimum_required(VERSION 3.0)
project(neurodraw)

set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_STANDARD 11)
set(THREADS_PREFER_PTHREAD_FLAG ON)
set(OpenGL_GL_PREFERENCE GLVND)

find_package(PkgConfig)
find_package(Threads REQUIRED)
find_package(SDL REQUIRED)
find_package(OpenGL REQUIRED)

include_directories(include
					${SDL_INCLUDE_DIR}
					${OPENGL_INCLUDE_DIRS})

add_library(${PROJECT_NAME} SHARED
			src/Palette.cpp
			src/Shape.cpp
			src/Rectangle.cpp
			src/Circle.cpp
			src/Ring.cpp
			src/Cross.cpp
			src/Arrow.cpp
			src/Triangle.cpp
			src/Line.cpp
			src/Arc.cpp
			src/RegularShape.cpp
			src/Thread.cpp
			src/Engine.cpp
			src/Window.cpp
			src/Rate.cpp
			)


add_library(neurochrono SHARED
			src/Rate.cpp)

add_executable(sandbox_single_thread src/sandbox_single_thread.cpp)
target_link_libraries(sandbox_single_thread ${PROJECT_NAME} 
											neurochrono
											${SDL_LIBRARY} 
											${OPENGL_LIBRARIES})

add_executable(sandbox_multi_thread src/sandbox_multi_thread.cpp)
target_link_libraries(sandbox_multi_thread ${PROJECT_NAME} 
										   neurochrono
										   ${SDL_LIBRARY} 
										   ${OPENGL_LIBRARIES} 
										   ${CMAKE_THREAD_LIBS_INIT})


install(TARGETS ${PROJECT_NAME}
	LIBRARY DESTINATION
)
install(DIRECTORY include/${PROJECT_NAME}
		DESTINATION include
		FILES_MATCHING 
		PATTERN "*.h"
		PATTERN "*.hpp")
									   
install(TARGETS neurochrono
	LIBRARY DESTINATION 
)
install(DIRECTORY include/neurochrono
		DESTINATION include
		FILES_MATCHING 
		PATTERN "*.h"
		PATTERN "*.hpp")
