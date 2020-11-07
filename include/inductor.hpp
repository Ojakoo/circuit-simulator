#pragma once

#include "component.hpp"


class Inductor: public Component {
    public:
        Inductor(const std::string& name,
        float inductance = 0,
        std::shared_ptr<Node> input = nullptr,
        std::shared_ptr<Node> output = nullptr);

        float GetValue() const;

        ComponentType GetType() const;

    private:
        float inductance_;  // henrys
};