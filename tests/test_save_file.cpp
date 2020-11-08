#include <string>
#include <sstream>
#include <fstream>

#include "doctest.h"
#include "save_and_load.hpp"
#include "circuit.hpp"


SCENARIO("Saving a circuit into a netlist file") {

  GIVEN("Circuit object and file location") {

    const std::string fname = "../../tests/netlists/netlist_good.txt";
    const std::string save_file_name = "netlist_save_test.txt";

    Circuit c = LoadNetList(fname);

    WHEN("The circuit is saved into a netlist file") {

      SaveNetList(c, save_file_name);

      THEN("The output file contains the correct information") {
        // open the file for reading
        std::ifstream is(save_file_name);
        std::stringstream buffer;
        buffer << is.rdbuf();
        CHECK(buffer.str() == "V V1 0 N001 5\nR R1 N001 N002 10\nC C1 N002 0 0.001\nL L1 0 N002 0.05");
      }
    }
  }
}
