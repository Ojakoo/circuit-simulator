#include <iostream>

#include "resistor.hpp"
#include "inductor.hpp"
#include "capacitor.hpp"
#include "dc_voltage_source.hpp"
#include "dc_current_source.hpp"
#include "circuit.hpp"
#include "save_and_load.hpp"


int main ( void ) {

    Circuit c = LoadNetList("../../tests/netlists/netlist_good.txt");

    SaveNetList(c, "l.txt");

    /*
    std::shared_ptr<Node> N001 = std::make_shared<Node>("N001");
    std::shared_ptr<Node> N002 = std::make_shared<Node>("N002");

    N002->SetNodeType(GROUND);

    Resistor R1 = Resistor("R1", 50);
    Inductor L1 = Inductor("L1", 0.001);
    Capacitor C1 = Capacitor("C1", 0.0009);
    DCVoltageSource V1 = DCVoltageSource("V1", 10);
    DCCurrentSource J1 = DCCurrentSource("J1", 2);

    R1.ConnectNodeToTerminal(N001, OUTPUT);
    R1.ConnectNodeToTerminal(N002, INPUT);

    L1.ConnectNodeToTerminal(N002, INPUT);

    std::cout << R1 << std::endl;
    std::cout << L1 << std::endl;
    std::cout << C1 << std::endl;
    std::cout << V1 << std::endl;
    std::cout << J1 << std::endl;
    */
    return 0;
}