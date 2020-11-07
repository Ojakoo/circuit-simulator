#include "inductor.hpp"


Inductor::Inductor(const std::string &name,
                  float inductance,
                  std::shared_ptr<Node> input,
                  std::shared_ptr<Node> output)
    : Component(name, input, output), inductance_(inductance) { }


float Inductor::GetValue() const {
    return inductance_;
}

ComponentType Inductor::GetType() const {
    return INDUCTOR;
}