file(GLOB TEST_SOURCES "${CMAKE_SOURCE_DIR}/src/*.cpp")
list(APPEND TEST_SOURCES "test_main.cpp")
list(REMOVE_ITEM TEST_SOURCES "${CMAKE_SOURCE_DIR}/src/main.cpp")
list(REMOVE_ITEM TEST_SOURCES "${CMAKE_SOURCE_DIR}/src/circuit_simulator_gui.cpp")
list(REMOVE_ITEM TEST_SOURCES "${CMAKE_SOURCE_DIR}/src/ImGuiFileBrowser.cpp")

add_library(test_main OBJECT ${TEST_SOURCES})

add_executable(circuit test_circuit.cpp $<TARGET_OBJECTS:test_main>)
add_test(NAME circuit COMMAND circuit)

add_executable(MNAsolver test_MNAsolver.cpp $<TARGET_OBJECTS:test_main>)
add_test(NAME MNAsolver COMMAND MNAsolver)
