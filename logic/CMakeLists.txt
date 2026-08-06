cmake_minimum_required(VERSION 3.20)

project(LogicCpp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(logic INTERFACE)

target_include_directories(logic
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

add_library(logic_gates STATIC
    examples/Gates.cpp
)

target_include_directories(logic_gates
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/examples
)

target_link_libraries(logic_gates
    PUBLIC
        logic
)

add_executable(logic_cpp
    examples/main.cpp
)

target_link_libraries(logic_cpp
    PRIVATE
        logic_gates
)

add_executable(test_piso
    examples/test_piso.cpp
)

target_link_libraries(test_piso
    PRIVATE
        logic
)

add_executable(test_shift_registers
    examples/test_shift_registers.cpp
)

target_link_libraries(test_shift_registers
    PRIVATE
        logic
)

add_executable(test_binary_counter
    examples/test_binary_counter.cpp
)

target_link_libraries(test_binary_counter
    PRIVATE
        logic
)


