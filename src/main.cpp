#include <iostream>

#include "component.hpp"


int main ( void ) {
    Component c = Component("Testi");
    std::cout << c.GetName() << std::endl;
    return 0;
}