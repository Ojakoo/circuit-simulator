#pragma once

#include "component.hpp"


class DCVoltageSource : public Component{
    public:
        DCVoltageSource(const std::string& name,
                        float voltage = 0,
                        std::shared_ptr<Node> input = nullptr,
                        std::shared_ptr<Node> output = nullptr);

        float GetValue() const;

        ComponentType GetType() const;

    private:
        float voltage_;  // volts
};