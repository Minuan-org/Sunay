cmake_minimum_required(VERSION 3.16)

project(Sunay
    VERSION 1.0.0
    DESCRIPTION "Sunay - Native Browser Rendering Library"
    LANGUAGES CXX
)

# ============================================================
# C++ Configuration
# ============================================================

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(GNUInstallDirs)

# ============================================================
# Platform Requirements
# ============================================================

if(NOT WIN32)
    message(FATAL_ERROR
        "Sunay currently supports Windows only. "
        "The Win32 renderer requires the Windows API."
    )
endif()

message(STATUS "Configuring Sunay ${PROJECT_VERSION}")
message(STATUS "Target platform: Windows")
message(STATUS "C++ standard: C++20")

# ============================================================
# Source Files
# ============================================================

set(SUNAY_SOURCES
    src/sunay.cpp
    src/win32_renderer.cpp
)

set(SUNAY_PUBLIC_HEADERS
    include/sunay.hpp
)

set(SUNAY_PRIVATE_HEADERS
    platform/win32/win32_renderer.hpp
)

# ============================================================
# Static Library
# ============================================================

add_library(Sunay STATIC
    ${SUNAY_SOURCES}
    ${SUNAY_PUBLIC_HEADERS}
    ${SUNAY_PRIVATE_HEADERS}
)

# Namespaced target for other CMake projects.
add_library(Sunay::Sunay ALIAS Sunay)

set_target_properties(Sunay PROPERTIES
    VERSION ${PROJECT_VERSION}
    OUTPUT_NAME "Sunay"
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)

# ============================================================
# Include Directories
# ============================================================

target_include_directories(Sunay
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>

    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/platform/win32
)

# ============================================================
# Platform Definitions
# ============================================================

target_compile_definitions(Sunay
    PRIVATE
        SUNAY_PLATFORM_WIN32
        WIN32_LEAN_AND_MEAN
        NOMINMAX
)

# ============================================================
# Windows Dependencies
# ============================================================

target_link_libraries(Sunay
    PUBLIC
        user32
        gdi32
)

# ============================================================
# Compiler Warnings
# ============================================================

if(MSVC)
    target_compile_options(Sunay PRIVATE
        /W4
        /permissive-
    )
else()
    target_compile_options(Sunay PRIVATE
        -Wall
        -Wextra
        -Wpedantic
    )
endif()

# ============================================================
# Build Information
# ============================================================

message(STATUS "Sunay build configuration:")
message(STATUS "  Version: ${PROJECT_VERSION}")
message(STATUS "  Library type: STATIC")
message(STATUS "  C++ standard: C++20")
message(STATUS "  Platform: Windows")
message(STATUS "  Output name: Sunay")

# ============================================================
# Installation
# ============================================================

install(
    TARGETS Sunay
    EXPORT SunayTargets

    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}

    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(
    FILES ${SUNAY_PUBLIC_HEADERS}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# Export CMake targets for other projects.
install(
    EXPORT SunayTargets
    FILE SunayTargets.cmake
    NAMESPACE Sunay::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Sunay
)

# ============================================================
# Configuration Complete
# ============================================================

message(STATUS "Sunay configuration complete!")
