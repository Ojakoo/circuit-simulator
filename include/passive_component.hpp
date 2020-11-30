#pragma once

#include "node.hpp"
#include "component.hpp"


class PassiveComponent : public Component {

    /*
    Passive components do not generate power.
    For example: resistors, inductors, capacitors
    and diodes. 
    */

    public:
        PassiveComponent(
            const std::string& name,
            std::shared_ptr<Node> input,
            std::shared_ptr<Node> output
        );

        ComponentClass GetClass() const;

        float GetCurrent() const;
        void SetCurrent(float value);

    private:
        float current_;

};