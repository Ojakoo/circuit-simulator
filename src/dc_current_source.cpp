#include "current_source.hpp"

CurrentSource::CurrentSource(const std::string &name,
                                float current,
                                std::shared_ptr<Node> input,
                                std::shared_ptr<Node> output)
    : ActiveComponent(name, input, output), current_(current) { }


float CurrentSource::GetValue() const {
    return current_;
}

void CurrentSource::SetValue(float newval) {
    current_ = newval;
}

ComponentType CurrentSource::GetType() const {
    return CURRENT_SOURCE;
}