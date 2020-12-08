#include <string>

#include "doctest.h"
#include "save_and_load.hpp"
#include "circuit.hpp"


SCENARIO("Reading circuit from a good file") {

  GIVEN("a good file to read") {
    const std::string fname = "../../tests/netlists/netlist_good.txt";

    WHEN("the good file is read") {
      Circuit c = LoadNetList(fname);
      
      THEN("There will be 4 components in the circuit") {
        CHECK(c.GetComponents().size() == 4);
      }

      AND_THEN("There will be 2 nodes in the circuit") {
        //CHECK(c.GetNodeCount() == 2);
      }

      AND_THEN("There will be 1 source in the circuit") {
        //CHECK(c.GetSourceCount() == 1);
      }
    }
  }
}


SCENARIO("Reading circuit from a bad file") {

  GIVEN("a bad file to read") {
    const std::string fname = "../../tests/netlists/netlist_bad.txt";

    WHEN("the bad file is read") {
      CHECK_THROWS(LoadNetList(fname));
    }
  }
}


SCENARIO("Reading invalid netlist path") {

  GIVEN("a random path to read") {
    const std::string fname = "../../tests/asddsadfsafd.txt";

    WHEN("the random path file is read") {
      CHECK_THROWS(LoadNetList(fname));
    }
  }
}


SCENARIO("Reading a good netlist but with invalid component type") {

  GIVEN("a good netlist to read") {
    const std::string fname = "../../tests/netlists/netlist_invalid_type.txt";

    WHEN("the netlist is read") {
      CHECK_THROWS(LoadNetList(fname));
    }
  }
}
