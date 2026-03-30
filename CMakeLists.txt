cmake_minimum_required(VERSION 3.16)
project(Sunay VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Public headers
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Engine sources
set(SUNAY_SOURCES
    src/sunay.cpp
    src/win32_renderer.cpp
)

add_library(Sunay STATIC ${SUNAY_SOURCES})

# Platform-specific dependencies
if (WIN32)
    target_compile_definitions(Sunay PRIVATE SUNAY_PLATFORM_WIN32)
    target_link_libraries(Sunay PRIVATE user32 gdi32)
endif()

# Install rules (optional)
install(TARGETS Sunay DESTINATION lib)
install(FILES include/sunay.hpp DESTINATION include)
