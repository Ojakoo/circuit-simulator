#include <iostream>

#include "resistor.hpp"
#include "inductor.hpp"
#include "capacitor.hpp"


int main ( void ) {

    Resistor R = Resistor("R1", 50);
    Inductor L = Inductor("L1", 0.001);
    Capacitor C = Capacitor("C1", 0.0009);

    std::cout << R << std::endl;
    std::cout << L << std::endl;
    std::cout << C << std::endl;

    return 0;
}