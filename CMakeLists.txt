cmake_minimum_required(VERSION 3.16)
project(neurodraw VERSION 2.0.0 LANGUAGES C CXX)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)
include(CTest)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(THREADS_PREFER_PTHREAD_FLAG ON)
set(OpenGL_GL_PREFERENCE GLVND)
option(NEURODRAW_BUILD_EXAMPLES "Build neurodraw example applications" ON)

find_package(Threads REQUIRED)
find_package(OpenGL REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL2 REQUIRED IMPORTED_TARGET sdl2)
pkg_check_modules(SDL2_IMAGE REQUIRED IMPORTED_TARGET SDL2_image)
pkg_check_modules(SDL2_TTF REQUIRED IMPORTED_TARGET SDL2_ttf)

set(NEURODRAW_CMAKE_DIR ${CMAKE_CURRENT_BINARY_DIR}/cmake)
set(NEURODRAW_INSTALL_CMAKEDIR ${CMAKE_INSTALL_LIBDIR}/cmake/neurodraw)

add_library(neurodraw SHARED
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
    src/Text.cpp
    src/Image.cpp
    src/Group.cpp
    src/Layer.cpp
    src/Tween.cpp
)
add_library(neurodraw::neurodraw ALIAS neurodraw)

target_include_directories(neurodraw
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

target_link_libraries(neurodraw
    PUBLIC
        PkgConfig::SDL2
        PkgConfig::SDL2_IMAGE
        PkgConfig::SDL2_TTF
        OpenGL::GL
)

add_library(neurochrono SHARED
    src/Rate.cpp
)
add_library(neurodraw::neurochrono ALIAS neurochrono)

target_include_directories(neurochrono
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

set_target_properties(neurodraw PROPERTIES EXPORT_NAME neurodraw)
set_target_properties(neurochrono PROPERTIES EXPORT_NAME neurochrono)
set_target_properties(neurodraw PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
)
set_target_properties(neurochrono PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
)

if(NEURODRAW_BUILD_EXAMPLES)
    add_executable(example_feature_showcase examples/example_feature_showcase.cpp)
    target_link_libraries(example_feature_showcase
        PRIVATE
            neurodraw
            neurochrono
    )

    add_executable(example_engine_showcase examples/example_engine_showcase.cpp)
    target_link_libraries(example_engine_showcase
        PRIVATE
            neurodraw
            neurochrono
            Threads::Threads
    )

    add_executable(example_gallery examples/example_gallery.cpp)
    target_link_libraries(example_gallery
        PRIVATE
            neurodraw
            neurochrono
    )

    add_executable(example_text_showcase examples/example_text_showcase.cpp)
    target_link_libraries(example_text_showcase
        PRIVATE
            neurodraw
            neurochrono
    )

    add_executable(example_image_showcase examples/example_image_showcase.cpp)
    target_link_libraries(example_image_showcase
        PRIVATE
            neurodraw
            neurochrono
    )
    target_compile_definitions(example_image_showcase
        PRIVATE
            NEURODRAW_EXAMPLE_ASSET_DIR="${CMAKE_CURRENT_SOURCE_DIR}/examples/assets"
    )

    add_executable(example_scene_showcase examples/example_scene_showcase.cpp)
    target_link_libraries(example_scene_showcase
        PRIVATE
            neurodraw
            neurochrono
            Threads::Threads
    )
endif()

configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/neurodrawConfig.cmake.in
    ${NEURODRAW_CMAKE_DIR}/neurodrawConfig.cmake
    INSTALL_DESTINATION ${NEURODRAW_INSTALL_CMAKEDIR}
)

write_basic_package_version_file(
    ${NEURODRAW_CMAKE_DIR}/neurodrawConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

export(
    TARGETS neurodraw neurochrono
    FILE ${NEURODRAW_CMAKE_DIR}/neurodrawTargets.cmake
    NAMESPACE neurodraw::
)

install(TARGETS neurodraw neurochrono
    EXPORT neurodrawTargets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

install(EXPORT neurodrawTargets
    FILE neurodrawTargets.cmake
    NAMESPACE neurodraw::
    DESTINATION ${NEURODRAW_INSTALL_CMAKEDIR}
)

install(FILES
    ${NEURODRAW_CMAKE_DIR}/neurodrawConfig.cmake
    ${NEURODRAW_CMAKE_DIR}/neurodrawConfigVersion.cmake
    DESTINATION ${NEURODRAW_INSTALL_CMAKEDIR}
)

install(DIRECTORY include/neurodraw
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    FILES_MATCHING
    PATTERN "*.h"
    PATTERN "*.hpp"
)

install(DIRECTORY include/neurochrono
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    FILES_MATCHING
    PATTERN "*.h"
    PATTERN "*.hpp"
)

if(BUILD_TESTING)
    add_executable(test_neurodraw_core tests/test_neurodraw_core.cpp)
    target_link_libraries(test_neurodraw_core
        PRIVATE
            neurodraw
            neurochrono
    )
    target_compile_definitions(test_neurodraw_core
        PRIVATE
            NEURODRAW_EXAMPLE_ASSET_DIR="${CMAKE_CURRENT_SOURCE_DIR}/examples/assets"
    )
    add_test(NAME neurodraw_core COMMAND test_neurodraw_core)

    add_executable(test_neurochrono tests/test_neurochrono.cpp)
    target_link_libraries(test_neurochrono PRIVATE neurochrono)
    add_test(NAME neurochrono_core COMMAND test_neurochrono)

    add_test(
        NAME neurodraw_package_consumer
        COMMAND ${CMAKE_COMMAND}
            -Dsource_dir=${CMAKE_CURRENT_SOURCE_DIR}/tests/package_consumer
            -Dbinary_dir=${CMAKE_CURRENT_BINARY_DIR}/tests/package_consumer
            -Dproject_build_dir=${CMAKE_CURRENT_BINARY_DIR}
            -Dpackage_subdir=${NEURODRAW_INSTALL_CMAKEDIR}
            -P ${CMAKE_CURRENT_SOURCE_DIR}/tests/package_consumer_smoke.cmake
    )
endif()
