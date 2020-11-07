#pragma once

#include "component.hpp"
#include <list>
#include <iostream>

class Circuit {
public:
    Circuit() {}

    const std::list<std::shared_ptr<Component>>& GetComponents() const;
    void AddComponent(std::shared_ptr<Component> component);
private:
    std::list<std::shared_ptr<Component>> components_;
};

std::ostream &operator<<(std::ostream& out, const Circuit& circuit);