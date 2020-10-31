#include "capacitor.hpp"


Capacitor::Capacitor(const std::string &name)
    : Component(name) { }


float Capacitor::GetValue() const {
    return capacitance_;
}

ComponentType Capacitor::GetType() const {
    return CAPACITOR;
}