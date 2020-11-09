#include <string>

#include "doctest.h"
#include "circuit.hpp"
#include "component.hpp"
#include "resistor.hpp"
#include "inductor.hpp"
#include "capacitor.hpp"
#include "dc_voltage_source.hpp"
#include "dc_current_source.hpp"
#include "node.hpp"

SCENARIO("Constructing circuit") {
    GIVEN("juu"){
        WHEN("jaa") {
            Circuit c = Circuit();

            std::shared_ptr<Node> n1 = c.AddNode("N001");
            std::shared_ptr<Node> n2 = c.AddNode("N002");
            std::shared_ptr<Node> n3 = c.AddNode("N003");

            std::shared_ptr<Resistor> r1 = std::make_shared<Resistor>("R1", 50, n1, n2);
            std::shared_ptr<Resistor> r2 = std::make_shared<Resistor>("R2", 50, n2, n3);

            c.AddComponent(r1);
            c.AddComponent(r2);

            THEN("There is 2 components in circuit") {
                CHECK(c.GetComponents().size() == 2);
            }
        }
    }
}

SCENARIO("Producing matrix") {
    GIVEN("hmm") {
        WHEN("dfsahj") {
            Circuit c = Circuit();

            std::shared_ptr<Node> n1 = c.AddNode("N001");
            std::shared_ptr<Node> n2 = c.AddNode("N002");
            std::shared_ptr<Node> g = c.AddNode("0");

            std::shared_ptr<Resistor> r1 = std::make_shared<Resistor>("R1", 50, n1, n2);
            std::shared_ptr<Capacitor> c1 = std::make_shared<Capacitor>("C1", 1, n2, g);
            std::shared_ptr<Inductor> l1 = std::make_shared<Inductor>("L1", 2, n2, g);
            std::shared_ptr<DCVoltageSource> s1 = std::make_shared<DCVoltageSource>("S1", 10, n1, g);

            c.AddComponent(r1);
            c.AddComponent(c1);
            c.AddComponent(l1);
            c.AddComponent(s1);

            THEN("There is 2 components in circuit") {
                CHECK(c.GetComponents().size() == 4);
            }
        }
    }
}