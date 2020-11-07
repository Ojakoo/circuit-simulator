#include "capacitor.hpp"


Capacitor::Capacitor(const std::string &name,
                    float capacitance,
                    std::shared_ptr<Node> input,
                    std::shared_ptr<Node> output)
    : Component(name, input, output), capacitance_(capacitance) { }


float Capacitor::GetValue() const {
    return capacitance_;
}

ComponentType Capacitor::GetType() const {
    return CAPACITOR;
}