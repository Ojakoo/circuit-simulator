#include "capacitor.hpp"


Capacitor::Capacitor(const std::string &name, float capacitance)
    : Component(name), capacitance_(capacitance) { }


float Capacitor::GetValue() const {
    return capacitance_;
}

ComponentType Capacitor::GetType() const {
    return CAPACITOR;
}