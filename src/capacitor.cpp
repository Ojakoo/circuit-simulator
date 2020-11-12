#include "capacitor.hpp"


Capacitor::Capacitor(const std::string &name,
                    float capacitance,
                    std::shared_ptr<Node> input,
                    std::shared_ptr<Node> output)
    : PassiveComponent(name, input, output), capacitance_(capacitance) { }


float Capacitor::GetValue() const {
    return capacitance_;
}

std::complex<float> Capacitor::GetAdmittance(const float w) const {
    return std::complex<float>(0, w * capacitance_);
}

ComponentType Capacitor::GetType() const {
    return CAPACITOR;
}