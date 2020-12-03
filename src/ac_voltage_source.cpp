#include "ac_voltage_source.hpp"

ACVoltageSource::ACVoltageSource(
        const std::string& name,
        float voltage,
        float frequency,
        std::shared_ptr<Node> input,
        std::shared_ptr<Node> output
    ) : ActiveComponent(name, input, output), voltage_(voltage), frequency_(frequency) { }

float ACVoltageSource::GetValue() const {
    return voltage_;
}

float ACVoltageSource::GetFrequency() const {
    return frequency_;
}

void ACVoltageSource::SetValue(float newval) {
    voltage_ = newval;
}

void ACVoltageSource::SetFrequency(float newval) {
    frequency_ = newval;
}

ComponentType ACVoltageSource::GetType() const {
    return AC_VOLTAGE_SOURCE;
}