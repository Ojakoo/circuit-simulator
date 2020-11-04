#include "resistor.hpp"


Resistor::Resistor(const std::string &name, float resistance)
    : Component(name), resistance_(resistance) { }


float Resistor::GetValue() const {
    return resistance_;
}

ComponentType Resistor::GetType() const {
    return RESISTOR;
}