file(GLOB TEST_SOURCES "${CMAKE_SOURCE_DIR}/src/*.cpp")
list(APPEND TEST_SOURCES "test_main.cpp")
list(REMOVE_ITEM TEST_SOURCES "${CMAKE_SOURCE_DIR}/src/main.cpp")
list(REMOVE_ITEM TEST_SOURCES "${CMAKE_SOURCE_DIR}/src/circuit_simulator_gui.cpp")
list(REMOVE_ITEM TEST_SOURCES "${CMAKE_SOURCE_DIR}/src/ImGuiFileBrowser.cpp")

add_library(test_main OBJECT ${TEST_SOURCES})

add_executable(load_file test_load_file.cpp ${TEST_SOURCES})
add_test(NAME load_file COMMAND load_file)

add_executable(save_file test_save_file.cpp $<TARGET_OBJECTS:test_main>)
add_test(NAME save_file COMMAND save_file)

add_executable(admittance test_admittance.cpp $<TARGET_OBJECTS:test_main>)
add_test(NAME admittance COMMAND admittance)

add_executable(circuit test_circuit.cpp $<TARGET_OBJECTS:test_main>)
add_test(NAME circuit COMMAND circuit)
