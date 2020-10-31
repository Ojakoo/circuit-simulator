#pragma once

#include "node.hpp"


enum ComponentType {
    RESISTOR,
    INDUCTOR,
    CAPACITOR
};


class Component {
    public:
        Component(const std::string& name);

        const std::string& GetName() const;

        virtual float GetValue() const = 0;

        virtual ComponentType GetType() const = 0;

        friend std::ostream &operator<<(std::ostream &out, const Component& c);

    private:
        std::string name_;
};