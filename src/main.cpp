#include <iostream>

#include "resistor.hpp"
#include "inductor.hpp"
#include "capacitor.hpp"
#include "dc_voltage_source.hpp"
#include "dc_current_source.hpp"


int main ( void ) {

    Resistor R1 = Resistor("R1", 50);
    Inductor L1 = Inductor("L1", 0.001);
    Capacitor C1 = Capacitor("C1", 0.0009);
    DCVoltageSource V1 = DCVoltageSource("V1", 10);
    DCCurrentSource J1 = DCCurrentSource("J1", 2);

    std::cout << R1 << std::endl;
    std::cout << L1 << std::endl;
    std::cout << C1 << std::endl;
    std::cout << V1 << std::endl;
    std::cout << J1 << std::endl;

    return 0;
}