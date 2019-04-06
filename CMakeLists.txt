cmake_minimum_required(VERSION 3.13)

#### Generic ####
# Show AND log compilation commands.
set(CMAKE_VERBOSE_MAKEFILE OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# Use C++17.
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Ofast -Wall -pedantic -Wextra")

project(PROJECTION)

# Dependencies

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

set(PROJECTION_SOURCES_HEADERS
        src/colorbalance.h
        src/remap.h)
set(PROJECTION_SOURCES ${PROJECTION_SOURCES_HEADERS}
        src/main.cpp
        src/remap.cpp)

add_executable(project ${PROJECTION_SOURCES})
target_include_directories(project PRIVATE src)
target_link_libraries( project PRIVATE ${OpenCV_LIBS} ${CONAN_LIBS})
