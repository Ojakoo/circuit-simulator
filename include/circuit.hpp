#pragma once

#include "component.hpp"
#include <list>
#include <iostream>

class Circuit {
public:
    Circuit() {}
    ~Circuit();
    const std::list<Component*>& GetComponents() const;
    void AddComponent(Component* component);
private:
    std::list<Component*> components_;
};

std::ostream &operator<<(std::ostream& out, const Circuit& circuit);