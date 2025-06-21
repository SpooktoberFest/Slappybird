cmake_minimum_required(VERSION 3.10)
project(SlappyBird)
set(EXECUTABLE_NAME "${PROJECT_NAME}.exe")

set(CMAKE_CXX_STANDARD 17)

# Find Libraries
find_package(raylib REQUIRED)

# Add Source Files
file(GLOB_RECURSE SOURCES "src/*.cpp")

# Set the output directory for executables
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin)

# Make Executable
add_executable(${EXECUTABLE_NAME} ${SOURCES})

# Add include path for header files
target_include_directories(${EXECUTABLE_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/includes)

# Link raylib (and dependencies if needed)
target_link_libraries(${EXECUTABLE_NAME} raylib)

# Add extra libraries if needed (Linux-specific dependencies)
if(UNIX AND NOT APPLE)
    target_link_libraries(${EXECUTABLE_NAME} m pthread dl)
endif()
