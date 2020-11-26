#pragma once

#include "active_component.hpp"


class DCVoltageSource : public ActiveComponent {
    public:
        DCVoltageSource(const std::string& name,
                        float voltage = 0,
                        std::shared_ptr<Node> input = nullptr,
                        std::shared_ptr<Node> output = nullptr);

        float GetValue() const;

        void SetValue(float newval);

        ComponentType GetType() const;

    private:
        float voltage_;  // volts
};