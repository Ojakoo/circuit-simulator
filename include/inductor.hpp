#pragma once

#include "component.hpp"


class Inductor: public Component {
    public:
        Inductor(const std::string& name);

        float GetValue() const;

        ComponentType GetType() const;

    private:
        float inductance_;  // henrys
};