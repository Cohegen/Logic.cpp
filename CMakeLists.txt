cmake_minimum_required(VERSION 3.20)

project(LogicCpp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Option settings
if(CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR)
    option(LOGIC_BUILD_EXAMPLES "Build Logic.cpp examples" ON)
    option(LOGIC_BUILD_TESTS "Build Logic.cpp component unit tests" ON)
else()
    option(LOGIC_BUILD_EXAMPLES "Build Logic.cpp examples" OFF)
    option(LOGIC_BUILD_TESTS "Build Logic.cpp component unit tests" OFF)
endif()

# Header-only library target
add_library(logic INTERFACE)
add_library(logic::logic ALIAS logic)

target_compile_features(logic
    INTERFACE
        cxx_std_20
)

target_include_directories(logic
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/logic>
        $<INSTALL_INTERFACE:include>
)

if(LOGIC_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

if(LOGIC_BUILD_TESTS)
    add_subdirectory(tests)
endif()
