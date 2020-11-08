add_library(test_main
            OBJECT
            test_main.cpp
            ../src/circuit.cpp
            ../src/save_and_load.cpp
            ../src/resistor.cpp
            ../src/capacitor.cpp
            ../src/component.cpp
            ../src/dc_current_source.cpp
            ../src/dc_voltage_source.cpp
            ../src/inductor.cpp
            ../src/node.cpp
            )

add_executable(load_file test_load_file.cpp $<TARGET_OBJECTS:test_main>)
add_test(NAME load_file COMMAND load_file)

add_executable(save_file test_save_file.cpp $<TARGET_OBJECTS:test_main>)
add_test(NAME save_file COMMAND save_file)
