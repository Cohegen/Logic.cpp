cmake_minimum_required(VERSION 3.20)
project(pylogic LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Find pybind11 (from python or system)
find_package(pybind11 CONFIG QUIET)

if(NOT pybind11_FOUND)
    include(FetchContent)
    FetchContent_Declare(
        pybind11
        GIT_REPOSITORY https://github.com/pybind/pybind11.git
        GIT_TAG v2.13.6
    )
    FetchContent_MakeAvailable(pybind11)
endif()

# Locate Logic.cpp include directories
set(LOGIC_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../include")

pybind11_add_module(_pylogic_core
    src/bindings.cpp
)

target_include_directories(_pylogic_core PRIVATE
    ${LOGIC_INCLUDE_DIR}
    ${LOGIC_INCLUDE_DIR}/logic
)

if(MINGW)
    target_link_options(_pylogic_core PRIVATE -static-libgcc -static-libstdc++ -Wl,-Bstatic -lwinpthread -Wl,-Bdynamic)
endif()

# Place compiled .pyd / .so directly into the pylogic/ package directory for instant importing
set_target_properties(_pylogic_core PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/pylogic"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/pylogic"
)
