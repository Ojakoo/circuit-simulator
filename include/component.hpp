#pragma once

#include <memory>

#include "node.hpp"

enum ComponentClass {
    PASSIVE,
    ACTIVE
};

enum ComponentType {
    RESISTOR,
    INDUCTOR,
    CAPACITOR,
    VOLTAGE_SOURCE,
    CURRENT_SOURCE
};


enum TerminalType {
    OUTPUT,
    INPUT
};


class Component {

    /*

    Base class for all circuit elements.

    Sources, resistors, capacitors and inductors
    all have unique name and the element might be connected
    to nodes via input and output terminals.

    */

    public:
        Component(
            const std::string& name,
            std::shared_ptr<Node> input,
            std::shared_ptr<Node> output
        );

        const std::string& GetName() const;

        const std::shared_ptr<Node> GetTerminalNode(TerminalType terminal) const;

        void ConnectNodeToTerminal(std::shared_ptr<Node> node, TerminalType terminal);

        virtual float GetValue() const = 0;

        virtual void SetValue(float newval) = 0;

        virtual ComponentClass GetClass() const = 0;

        virtual ComponentType GetType() const = 0;

        friend std::ostream &operator<<(std::ostream &out, const Component& c);

    private:
        std::string name_;
        std::shared_ptr<Node> output_;
        std::shared_ptr<Node> input_;
};