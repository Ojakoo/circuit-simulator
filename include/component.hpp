#pragma once

#include "node.hpp"


class Component {
    public:
        Component(const std::string& name);

        const std::string& GetName() const;

    private:
        std::string name_;
};