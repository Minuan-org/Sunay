cmake_minimum_required(VERSION 3.16)

project(Sunay
    VERSION 1.0.1
    DESCRIPTION "Sunay - embedded web rendering pipeline for Minuan"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_AUTOMOC ON)

include(GNUInstallDirs)
include(CTest)

find_package(QT NAMES Qt6 Qt5 REQUIRED
    COMPONENTS Core Widgets WebEngineWidgets
)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED
    COMPONENTS Core Widgets WebEngineWidgets
)

set(SUNAY_PUBLIC_HEADERS include/sunay/rendering_pipeline.hpp)

add_library(Sunay STATIC
    src/rendering_pipeline.cpp
    ${SUNAY_PUBLIC_HEADERS}
)
add_library(Sunay::Sunay ALIAS Sunay)

set_target_properties(Sunay PROPERTIES
    VERSION ${PROJECT_VERSION}
    OUTPUT_NAME "Sunay"
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
    AUTOMOC ON
)

target_include_directories(Sunay
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

target_link_libraries(Sunay
    PUBLIC
        Qt${QT_VERSION_MAJOR}::Core
        Qt${QT_VERSION_MAJOR}::Widgets
        Qt${QT_VERSION_MAJOR}::WebEngineWidgets
)

if(MSVC)
    target_compile_options(Sunay PRIVATE /W4 /permissive-)
else()
    target_compile_options(Sunay PRIVATE -Wall -Wextra -Wpedantic)
endif()

if(BUILD_TESTING)
    add_executable(SunayPipelineTests
        tests/rendering_pipeline_test.cpp
    )
    target_link_libraries(SunayPipelineTests PRIVATE Sunay::Sunay)
    if(MSVC)
        target_compile_options(SunayPipelineTests PRIVATE /W4 /permissive-)
    else()
        target_compile_options(SunayPipelineTests PRIVATE -Wall -Wextra -Wpedantic)
    endif()
    add_test(NAME SunayRenderingPipeline COMMAND SunayPipelineTests)
endif()

message(STATUS "Configuring Sunay ${PROJECT_VERSION}")
message(STATUS "Qt version: ${QT_VERSION}")
message(STATUS "Rendering backend: Qt WebEngine / Chromium")

install(
    TARGETS Sunay
    EXPORT SunayTargets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(
    FILES ${SUNAY_PUBLIC_HEADERS}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/sunay
)

install(
    EXPORT SunayTargets
    FILE SunayTargets.cmake
    NAMESPACE Sunay::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Sunay
)
