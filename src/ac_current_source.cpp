#include "ac_current_source.hpp"

ACCurrentSource::ACCurrentSource(
        const std::string& name,
        float current,
        float frequency,
        std::shared_ptr<Node> input,
        std::shared_ptr<Node> output
    ) : ActiveComponent(name, input, output), current_(current), frequency_(frequency) { }

float ACCurrentSource::GetValue() const {
    return current_;
}

float ACCurrentSource::GetFrequency() const {
    return frequency_;
}

void ACCurrentSource::SetValue(float newval) {
    current_ = newval;
}

void ACCurrentSource::SetFrequency(float newval) {
    frequency_ = newval;
}

ComponentType ACCurrentSource::GetType() const {
    return AC_CURRENT_SOURCE;
}