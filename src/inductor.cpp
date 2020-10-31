#include "inductor.hpp"


Inductor::Inductor(const std::string &name, float inductance)
    : Component(name), inductance_(inductance) { }


float Inductor::GetValue() const {
    return inductance_;
}

ComponentType Inductor::GetType() const {
    return INDUCTOR;
}