#include "dc_current_source.hpp"

DCCurrentSource::DCCurrentSource(const std::string &name,
                                float current,
                                std::shared_ptr<Node> input,
                                std::shared_ptr<Node> output)
    : ActiveComponent(name, input, output), current_(current) { }


float DCCurrentSource::GetValue() const {
    return current_;
}

ComponentType DCCurrentSource::GetType() const {
    return DC_CURRENT_SOURCE;
}