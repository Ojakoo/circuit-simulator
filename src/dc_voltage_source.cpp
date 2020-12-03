#include "voltage_source.hpp"

VoltageSource::VoltageSource(const std::string &name,
                                float voltage,
                                std::shared_ptr<Node> input,
                                std::shared_ptr<Node> output)
    : ActiveComponent(name, input, output), voltage_(voltage) { }


float VoltageSource::GetValue() const {
    return voltage_;
}

void VoltageSource::SetValue(float newval) {
    voltage_ = newval;
}

ComponentType VoltageSource::GetType() const {
    return VOLTAGE_SOURCE;
}