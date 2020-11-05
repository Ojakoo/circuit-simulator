#include <string>

#include "doctest.h"
#include "read_file.hpp"
#include "resistor.hpp"


SCENARIO("Reading circuit from a good file") {

  GIVEN("a good file to read") {
    std::string fname = "netlist.txt";

    Resistor r = Resistor("r");

    WHEN("the good file is read") {
      ReadCircuitFromFile(fname);
      
      THEN("it's expected nobody interupted our rest") {
        CHECK(1 == 0);
      }
      
    }
  }
} 