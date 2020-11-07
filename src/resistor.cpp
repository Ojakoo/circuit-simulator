#include "resistor.hpp"


Resistor::Resistor(const std::string &name,
                  float resistance,
                  std::shared_ptr<Node> input,
                  std::shared_ptr<Node> output)
    : Component(name, input, output), resistance_(resistance) { }


float Resistor::GetValue() const {
    return resistance_;
}

ComponentType Resistor::GetType() const {
    return RESISTOR;
}