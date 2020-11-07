#include "dc_voltage_source.hpp"

DCVoltageSource::DCVoltageSource(const std::string &name,
                                float voltage,
                                std::shared_ptr<Node> input,
                                std::shared_ptr<Node> output)
    : Component(name, input, output), voltage_(voltage) { }


float DCVoltageSource::GetValue() const {
    return voltage_;
}

ComponentType DCVoltageSource::GetType() const {
    return DC_VOLTAGE_SOURCE;
}