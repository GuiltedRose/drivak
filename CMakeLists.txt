cmake_minimum_required(VERSION 3.16)

# Project Identity
project(Drivak VERSION 0.1.0 LANGUAGES CXX)
message(STATUS "Forging Drivak...")

# Standard Setup
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Output Settings
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# OS Platform Detection
if(WIN32)
    set(DRIVAK_PLATFORM "Windows")
elseif(UNIX AND NOT APPLE)
    set(DRIVAK_PLATFORM "Linux")
elseif(APPLE)
    set(DRIVAK_PLATFORM "macOS")
else()
    set(DRIVAK_PLATFORM "Unknown")
endif()
message(STATUS "Target Platform: ${DRIVAK_PLATFORM}")

message(STATUS "Linking spell scrolls...")

# Include headers
include_directories(${PROJECT_SOURCE_DIR}/include)

# ==== Source Grouping by Module ====

# GameDB (Database Core)
file(GLOB_RECURSE GAMEDB_SRC CONFIGURE_DEPENDS src/gamedb/*.cpp)
file(GLOB_RECURSE GAMEDB_HEADERS CONFIGURE_DEPENDS include/gamedb/*.h)

# Core Logic
file(GLOB_RECURSE CORE_SRC CONFIGURE_DEPENDS src/core/*.cpp)
file(GLOB_RECURSE CORE_HEADERS CONFIGURE_DEPENDS include/core/*.h)

# Config Logic
file(GLOB_RECURSE CONFIG_SRC CONFIGURE_DEPENDS src/config/*.cpp)
file(GLOB_RECURSE CONFIG_HEADERS CONFIGURE_DEPENDS include/config/*.h)

# Modular Tools
file(GLOB_RECURSE TOOLS_SRC CONFIGURE_DEPENDS src/tools/*.cpp)
file(GLOB_RECURSE TOOLS_HEADERS CONFIGURE_DEPENDS include/tools/*.h)

# Modding System
file(GLOB_RECURSE MODDING_SRC CONFIGURE_DEPENDS src/modding/*.cpp)
file(GLOB_RECURSE MODDING_HEADERS CONFIGURE_DEPENDS include/modding/*.h)

# UI (Qt Screens)
file(GLOB_RECURSE UI_SRC CONFIGURE_DEPENDS src/ui/*.cpp)
file(GLOB_RECURSE UI_HEADERS CONFIGURE_DEPENDS include/ui/*.h)

# Renderer (Vulkan)
file(GLOB_RECURSE RENDERER_SRC CONFIGURE_DEPENDS src/renderer/*.cpp)
file(GLOB_RECURSE RENDERER_HEADERS CONFIGURE_DEPENDS include/renderer/*.h)

# Screens (Menu Items)
file(GLOB_RECURSE SCREENS_SRC CONFIGURE_DEPENDS src/screens/*.cpp)
file(GLOB_RECURSE SCREENS_HEADERS CONFIGURE_DEPENDS include/screens/*.h)

# Main Entry Point
set(MAIN_SRC src/main.cpp)

# ==== Merge All Sources & Headers ====
set(DRIVAK_SOURCES
    ${MAIN_SRC}
    ${GAMEDB_SRC}
    ${CORE_SRC}
    ${CONFIG_SRC}
    ${TOOLS_SRC}
    ${MODDING_SRC}
    ${UI_SRC}
    ${RENDERER_SRC}
    ${SCREENS_SRC}
)

set(DRIVAK_HEADERS
    ${GAMEDB_HEADERS}
    ${CORE_HEADERS}
    ${CONFIG_HEADERS}
    ${TOOLS_HEADERS}
    ${MODDING_HEADERS}
    ${UI_HEADERS}
    ${RENDERER_HEADERS}
    ${SCREENS_HEADERS}
)

message(STATUS "Casting `find_package` for Qt and Python bindings...")

# ==== Qt6 ====
find_package(Qt6 REQUIRED COMPONENTS Core Widgets Gui)
message(STATUS "Qt6 found, assembling the UI.")

# ==== Python ====
find_package(Python3 COMPONENTS Interpreter Development REQUIRED)
message(STATUS "Python3 embedded runtime located.")

# ==== Build the Executable ====
add_executable(drivak ${DRIVAK_SOURCES} ${DRIVAK_HEADERS})

# ==== Link Required Libraries ====
target_link_libraries(drivak
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    ${Python3_LIBRARIES}
)

# Include Python headers
target_include_directories(drivak PRIVATE ${Python3_INCLUDE_DIRS})

# ==== Compiler Warnings ====
if(MSVC)
    target_compile_options(drivak PRIVATE /W4 /permissive- /std:c++20)
else()
    target_compile_options(drivak PRIVATE -Wall -Wextra -pedantic)
endif()

# ==== Installation Target ====
install(TARGETS drivak RUNTIME DESTINATION bin)

# ==== Installation Target ====
install(TARGETS drivak RUNTIME DESTINATION bin)

message(STATUS "Drivak is forged and ready.")