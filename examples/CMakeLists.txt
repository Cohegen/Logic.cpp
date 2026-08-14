add_library(logic_gates STATIC
    Gates.cpp
)

target_include_directories(logic_gates
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(logic_gates
    PUBLIC
        logic::logic
)

add_executable(logic_cpp
    main.cpp
)

target_link_libraries(logic_cpp
    PRIVATE
        logic_gates
)
