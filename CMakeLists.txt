cmake_minimum_required(VERSION 3.15)
project(NES_Emulator VERSION 1.0.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Include directories
include_directories(include)

# Core library sources (excluding main.cpp)
file(GLOB_RECURSE CORE_SOURCES
    "src/core/*.cpp"
)

# Create static library for core emulator code
add_library(nes_core STATIC ${CORE_SOURCES})
target_include_directories(nes_core PUBLIC include)
target_compile_options(nes_core PRIVATE -Wall -Wextra)

# Main executable (only main.cpp)
add_executable(${PROJECT_NAME} src/main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE nes_core)

# Enable testing
enable_testing()

# Add tests subdirectory if exists
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/tests")
    add_subdirectory(tests)
endif()
