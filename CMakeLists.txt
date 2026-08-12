cmake_minimum_required(VERSION 3.20)

project(LogicCpp LANGUAGES CXX)

if(CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR)
    set(LOGIC_BUILD_EXAMPLES_DEFAULT ON)
else()
    set(LOGIC_BUILD_EXAMPLES_DEFAULT OFF)
endif()

option(LOGIC_BUILD_EXAMPLES "Build Logic.cpp examples and component tests" ${LOGIC_BUILD_EXAMPLES_DEFAULT})

add_subdirectory(logic)
