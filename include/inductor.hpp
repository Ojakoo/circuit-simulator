#pragma once

#include <complex>

#include "passive_component.hpp"


class Inductor: public PassiveComponent {
    public:
        Inductor(const std::string& name,
        float inductance = 0,
        std::shared_ptr<Node> input = nullptr,
        std::shared_ptr<Node> output = nullptr);

        float GetValue() const;

        std::complex<float> GetAdmittance(const float w) const;

        ComponentType GetType() const;

    private:
        float inductance_;  // henrys
};