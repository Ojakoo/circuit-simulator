#include "resistor.hpp"


Resistor::Resistor(const std::string &name)
    : Component(name) { }


float Resistor::GetValue() const {
    return resistance_;
}

ComponentType Resistor::GetType() const {
    return RESISTOR;
}