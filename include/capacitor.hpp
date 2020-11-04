#pragma once

#include "component.hpp"


class Capacitor: public Component {
    public:
        Capacitor(const std::string& name, float capacitance = 0);

        float GetValue() const;

        ComponentType GetType() const;

    private:
        float capacitance_;  // farads
};