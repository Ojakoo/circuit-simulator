#include <string>

#include "doctest.h"
#include "read_file.hpp"
#include "circuit.hpp"


SCENARIO("Reading circuit from a good file") {

  GIVEN("a good file to read") {
    const std::string fname = "../../tests/netlist.txt";

    WHEN("the good file is read") {
      Circuit c = ReadCircuitFromFile(fname);
      
      THEN("There will be 4 components in the circuit") {
        CHECK(c.GetComponents().size() == 4);
      }
      
    }
  }
} 