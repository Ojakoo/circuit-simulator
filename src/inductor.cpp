#include "inductor.hpp"


Inductor::Inductor(const std::string &name)
    : Component(name) { }


float Inductor::GetValue() const {
    return inductance_;
}

ComponentType Inductor::GetType() const {
    return INDUCTOR;
}