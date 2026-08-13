cmake_minimum_required(VERSION 3.20)

project(LogicCpp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT DEFINED LOGIC_BUILD_EXAMPLES)
    option(LOGIC_BUILD_EXAMPLES "Build Logic.cpp examples and component tests" OFF)
endif()

add_library(logic INTERFACE)
add_library(logic::logic ALIAS logic)

target_compile_features(logic
    INTERFACE
        cxx_std_20
)

target_include_directories(logic
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

if(LOGIC_BUILD_EXAMPLES)
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

add_executable(test_up_down_counter
    examples/test_up_down_counter.cpp
)

target_link_libraries(test_up_down_counter
    PRIVATE
        logic
)

add_executable(test_modulo_counter
    examples/test_modulo_counter.cpp
)

target_link_libraries(test_modulo_counter
    PRIVATE
        logic
)

add_executable(test_johnson_counter
    examples/test_johnson_counter.cpp
)

target_link_libraries(test_johnson_counter
    PRIVATE
        logic
)

add_executable(test_ring_counter
    examples/test_ring_counter.cpp
)

target_link_libraries(test_ring_counter
    PRIVATE
        logic
)

add_executable(test_ram
    examples/test_ram.cpp
)

target_link_libraries(test_ram
    PRIVATE
        logic
)

add_executable(test_rom
    examples/test_rom.cpp
)

target_link_libraries(test_rom
    PRIVATE
        logic
)

add_executable(test_register_file
    examples/test_register_file.cpp
)

target_link_libraries(test_register_file
    PRIVATE
        logic
)

add_executable(test_memory_controller
    examples/test_memory_controller.cpp
)

target_link_libraries(test_memory_controller
    PRIVATE
        logic
)

add_executable(test_memory
    examples/test_memory.cpp
)

target_link_libraries(test_memory
    PRIVATE
        logic
)

add_executable(test_expanded_components
    examples/test_expanded_components.cpp
)

target_link_libraries(test_expanded_components
    PRIVATE
        logic
)
endif()
