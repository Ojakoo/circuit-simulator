#pragma once

#include <complex>

#include "passive_component.hpp"


class Capacitor: public PassiveComponent {
    public:
        Capacitor(const std::string& name,
                  float capacitance = 0,
                  std::shared_ptr<Node> input = nullptr,
                  std::shared_ptr<Node> output = nullptr);

        float GetValue() const;

        void SetValue(float newval);

        std::complex<float> GetAdmittance(const float w) const;

        ComponentType GetType() const;

    private:
        float capacitance_;  // farads
};