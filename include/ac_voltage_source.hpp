#pragma once

#include "active_component.hpp"


class ACVoltageSource : public ActiveComponent {
    public:
        ACVoltageSource(
            const std::string& name,
            float voltage = 0,
            float frequency = 0,
            std::shared_ptr<Node> input = nullptr,
            std::shared_ptr<Node> output = nullptr
        );

        float GetValue() const;

        float GetFrequency() const;

        void SetValue(float newval);

        void SetFrequency(float newval);

        ComponentType GetType() const;

    private:
        float voltage_;  // volts
        float frequency_; // hertz
};