add_executable(test_piso test_piso.cpp)
target_link_libraries(test_piso PRIVATE logic::logic)

add_executable(test_shift_registers test_shift_registers.cpp)
target_link_libraries(test_shift_registers PRIVATE logic::logic)

add_executable(test_binary_counter test_binary_counter.cpp)
target_link_libraries(test_binary_counter PRIVATE logic::logic)

add_executable(test_up_down_counter test_up_down_counter.cpp)
target_link_libraries(test_up_down_counter PRIVATE logic::logic)

add_executable(test_modulo_counter test_modulo_counter.cpp)
target_link_libraries(test_modulo_counter PRIVATE logic::logic)

add_executable(test_johnson_counter test_johnson_counter.cpp)
target_link_libraries(test_johnson_counter PRIVATE logic::logic)

add_executable(test_ring_counter test_ring_counter.cpp)
target_link_libraries(test_ring_counter PRIVATE logic::logic)

add_executable(test_ram test_ram.cpp)
target_link_libraries(test_ram PRIVATE logic::logic)

add_executable(test_rom test_rom.cpp)
target_link_libraries(test_rom PRIVATE logic::logic)

add_executable(test_register_file test_register_file.cpp)
target_link_libraries(test_register_file PRIVATE logic::logic)

add_executable(test_memory_controller test_memory_controller.cpp)
target_link_libraries(test_memory_controller PRIVATE logic::logic)

add_executable(test_memory test_memory.cpp)
target_link_libraries(test_memory PRIVATE logic::logic)

add_executable(test_expanded_components test_expanded_components.cpp)
target_link_libraries(test_expanded_components PRIVATE logic::logic)
