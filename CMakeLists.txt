cmake_minimum_required(VERSION 3.10)
project(SuperFlappy)

if(WIN32)
    set(EXECUTABLE_NAME "SuperFlappy.exe")
else()
    set(EXECUTABLE_NAME "SuperFlappy")
endif()

set(CMAKE_CXX_STANDARD 17)

find_package(raylib REQUIRED)

file(GLOB_RECURSE SOURCES "src/*.cpp")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin)

add_executable(${EXECUTABLE_NAME} ${SOURCES})

target_include_directories(${EXECUTABLE_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/includes/containers
    ${CMAKE_SOURCE_DIR}/includes/world
    ${CMAKE_SOURCE_DIR}/includes/globals
    ${CMAKE_SOURCE_DIR}/third_party
    /usr/local/include/json/single_include/nlohmann/
)

target_link_libraries(${EXECUTABLE_NAME}
    raylib
)

if(UNIX AND NOT APPLE)
    target_link_libraries(${EXECUTABLE_NAME} m pthread dl)
endif()

