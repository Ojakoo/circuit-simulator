#pragma once

#include "node.hpp"
#include "component.hpp"


class ActiveComponent : public Component {

    /*
    Active components generate power into the circuit.
    For example: independent and dependent voltage and
    current sources.
    */

    public:
        ActiveComponent(
            const std::string& name,
            std::shared_ptr<Node> input,
            std::shared_ptr<Node> output
        );

        ComponentClass GetClass() const;
};