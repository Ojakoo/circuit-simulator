#include <iostream>

#include "resistor.hpp"
#include "inductor.hpp"
#include "capacitor.hpp"


int main ( void ) {
    Resistor R = Resistor("R1");
    Inductor L = Inductor("L1");
    Capacitor C = Capacitor("C1"); 
    std::cout << R.GetName() << R.GetType() << R.GetValue() << std::endl;
    std::cout << L.GetName() << L.GetType() << L.GetValue() << std::endl;
    std::cout << C.GetName() << C.GetType() << C.GetValue() << std::endl;
    return 0;
}