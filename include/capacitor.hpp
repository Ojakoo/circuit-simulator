#pragma once

#include "component.hpp"


class Capacitor: public Component {
    public:
        Capacitor(const std::string& name,
                  float capacitance = 0,
                  std::shared_ptr<Node> input = nullptr,
                  std::shared_ptr<Node> output = nullptr);

        float GetValue() const;

        ComponentType GetType() const;

    private:
        float capacitance_;  // farads
};