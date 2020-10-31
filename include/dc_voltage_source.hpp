#pragma once

#include "component.hpp"


class DCVoltageSource : public Component{
    public:
        DCVoltageSource(const std::string& name, float voltage);

        float GetValue() const;

        ComponentType GetType() const;

    private:
        float voltage_;  // volts
};