#include "dc_current_source.hpp"

DCCurrentSource::DCCurrentSource(const std::string &name, float current)
    : Component(name), current_(current) { }


float DCCurrentSource::GetValue() const {
    return current_;
}

ComponentType DCCurrentSource::GetType() const {
    return DC_CURRENT_SOURCE;
}