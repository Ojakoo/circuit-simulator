#include "inductor.hpp"


Inductor::Inductor(const std::string &name,
                  float inductance,
                  std::shared_ptr<Node> input,
                  std::shared_ptr<Node> output)
    : PassiveComponent(name, input, output), inductance_(inductance) { }


float Inductor::GetValue() const {
    return inductance_;
}

void Inductor::SetValue(float newval) {
    inductance_ = newval;
}

std::complex<float> Inductor::GetAdmittance(const float w) const {
    return std::complex<float>(0, 1 / (w * inductance_));
}

ComponentType Inductor::GetType() const {
    return INDUCTOR;
}