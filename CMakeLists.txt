# Version check
cmake_minimum_required (VERSION 3.0)

set(data-plot "data_plot")

# Project
project(${data-plot})

# Search Directories
include_directories("..")

# Add files
file(GLOB sources "*.cpp")
add_library(${data-plot} STATIC ${sources})

# Libraries
find_library(LCAIROWRAP NAMES cairo_wrap PATHS "../cairo-wrap/build" "cairo-wrap/build" STATIC)

message("-- External libraries")
message("	${LCAIROWRAP}")

target_link_libraries(${data-plot} ${LCAIROWRAP} cairo)

# Compiler Options
target_compile_options(${data-plot} PRIVATE -fexceptions -std=c++14 -O3 -Wall -pedantic-errors -pedantic)