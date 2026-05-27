cmake_minimum_required(VERSION 3.20)

project(LogicCpp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(logic_gates
    gates/Gates.cpp
)

target_include_directories(logic_gates
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/gates
        ${CMAKE_CURRENT_SOURCE_DIR}/combinational
)

add_executable(logic_cpp
    src/main.cpp
)

target_link_libraries(logic_cpp
    PRIVATE
        logic_gates
)
