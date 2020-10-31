#include "component.hpp"


Component::Component(const std::string &name)
    : name_(name) { }

const std::string& Component::GetName() const {
    return name_;
}