#include <iostream>

#include "component.hpp"


Component::Component(const std::string &name)
    : name_(name) { }

const std::string& Component::GetName() const {
    return name_;
}

/*

int main ( void ) {
    Component c = Component("Testi");
    std::cout << c.GetName() << std::endl;
    return 0;
}

*/