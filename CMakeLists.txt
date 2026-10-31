cmake_minimum_required(VERSION 3.16)

project(Sunay
    VERSION 1.0.0
    DESCRIPTION "Sunay - Minuan's embedded web rendering pipeline"
    LANGUAGES CXX
)

# C++ configuration
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# The rendering pipeline uses Qt signals and slots.
set(CMAKE_AUTOMOC ON)

include(GNUInstallDirs)

# Current native UI renderer is Win32-specific.
if(NOT WIN32)
    message(FATAL_ERROR
        "Sunay currently supports Windows only. "
        "The native UI renderer requires the Windows API."
    )
endif()

# Qt WebEngine provides the embedded Chromium page renderer.
find_package(Qt6 COMPONENTS Core Widgets WebEngineWidgets QUIET)
if(Qt6_FOUND)
    set(SUNAY_QT_PACKAGE Qt6)
else()
    find_package(Qt5 COMPONENTS Core Widgets WebEngineWidgets REQUIRED)
    set(SUNAY_QT_PACKAGE Qt5)
endif()

message(STATUS "Configuring Sunay ${PROJECT_VERSION}")
message(STATUS "Using ${SUNAY_QT_PACKAGE} WebEngine")

set(SUNAY_SOURCES
    src/sunay.cpp
    src/win32_renderer.cpp
    src/rendering_pipeline.cpp
)

set(SUNAY_PUBLIC_HEADERS
    include/sunay.hpp
    include/sunay/rendering_pipeline.hpp
)

set(SUNAY_PRIVATE_HEADERS
    platform/win32/win32_renderer.hpp
)

add_library(Sunay STATIC
    ${SUNAY_SOURCES}
    ${SUNAY_PUBLIC_HEADERS}
    ${SUNAY_PRIVATE_HEADERS}
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
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/platform/win32
)

target_compile_definitions(Sunay PRIVATE
    SUNAY_PLATFORM_WIN32
    WIN32_LEAN_AND_MEAN
    NOMINMAX
)

# Qt dependencies are PUBLIC because Sunay's public rendering header exposes
# QObject, QPointer, QString and QUrl in its API.
target_link_libraries(Sunay
    PUBLIC
        ${SUNAY_QT_PACKAGE}::Core
        ${SUNAY_QT_PACKAGE}::Widgets
        ${SUNAY_QT_PACKAGE}::WebEngineWidgets
        user32
        gdi32
)

if(MSVC)
    target_compile_options(Sunay PRIVATE /W4 /permissive-)
else()
    target_compile_options(Sunay PRIVATE -Wall -Wextra -Wpedantic)
endif()

message(STATUS "Sunay build configuration:")
message(STATUS "  Version: ${PROJECT_VERSION}")
message(STATUS "  Library type: STATIC")
message(STATUS "  C++ standard: C++20")
message(STATUS "  Platform: Windows")
message(STATUS "  Qt package: ${SUNAY_QT_PACKAGE}")

install(
    TARGETS Sunay
    EXPORT SunayTargets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# Preserve the public include/sunay/... directory structure.
install(
    DIRECTORY include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(
    EXPORT SunayTargets
    FILE SunayTargets.cmake
    NAMESPACE Sunay::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Sunay
)

message(STATUS "Sunay configuration complete.")
