add_library(test_main
            OBJECT
            test_main.cpp
            ../src/circuit.cpp
            ../src/read_file.cpp
            ../src/resistor.cpp
            ../src/capacitor.cpp
            ../src/component.cpp
            ../src/dc_current_source.cpp
            ../src/dc_voltage_source.cpp
            ../src/inductor.cpp
            ../src/node.cpp
            )

add_executable(test1 test_read_file.cpp $<TARGET_OBJECTS:test_main>)
add_test(NAME test1 COMMAND test1)

add_executable(test2 test2.cpp $<TARGET_OBJECTS:test_main>)
add_test(NAME test2 COMMAND test2 )
