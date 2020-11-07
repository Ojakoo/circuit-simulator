#pragma once

#include <list>
#include <iostream>
#include <map>

#include "component.hpp"
#include "node.hpp"


class Circuit {
public:
    Circuit() {}

    const std::list<std::shared_ptr<Component>>& GetComponents() const;
    const int GetNodeCount() const;
    const int GetSourceCount() const;
    const std::shared_ptr<Node> AddNode(const std::string& node_name);
    void AddComponent(std::shared_ptr<Component> component);
private:
    int m_ = 0;  // number of sources
    int n_ = 0;  // number of nodes (excluding ground node)
    std::map<std::string, std::shared_ptr<Node>> nodes_;
    std::list<std::shared_ptr<Component>> components_;
};

std::ostream &operator<<(std::ostream& out, const Circuit& circuit);