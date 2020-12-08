#include "ac_current_source.hpp"

ACCurrentSource::ACCurrentSource(
        const std::string& name,
        float current,
        std::shared_ptr<Node> input,
        std::shared_ptr<Node> output
    ) : ActiveComponent(name, input, output), current_(current) { }

float ACCurrentSource::GetValue() const {
    return current_;
}

void ACCurrentSource::SetValue(float newval) {
    current_ = newval;
}

ComponentType ACCurrentSource::GetType() const {
    return AC_CURRENT_SOURCE;
}