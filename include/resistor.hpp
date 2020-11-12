#pragma once

#include <complex>

#include "passive_component.hpp"


class Resistor: public PassiveComponent {
    public:
        Resistor(const std::string& name,
                float resistance = 0,
                std::shared_ptr<Node> input = nullptr,
                std::shared_ptr<Node> output = nullptr);

        float GetValue() const;

        float GetAdmittance() const;

        ComponentType GetType() const;

    private:
        float resistance_;  // ohms
};