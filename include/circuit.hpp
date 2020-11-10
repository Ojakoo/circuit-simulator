#pragma once

#include <list>
#include <iostream>
#include <map>

#include "component.hpp"
#include "node.hpp"
#include "Eigen/Dense"
using namespace Eigen;

class Circuit {
public:
    Circuit() {}

    const std::list<std::shared_ptr<Component>>& GetComponents() const;
    const MatrixXcf sMatrix() const; //getting matrix for MNAsolver::solveSteady
    const int GetNodeCount() const;
    const int GetSourceCount() const;
    const int GetVoltageSourceCount() const;
    const std::shared_ptr<Node> AddNode(const std::string& node_name);
    void AddComponent(std::shared_ptr<Component> component);
private:
    int i_ = 0; // number of sources
    int n_ = 0; // number of nodes (excluding ground node)
    int m_ = 0; // number of voltage sources
    std::map<std::string, std::shared_ptr<Node>> nodes_;
    std::list<std::shared_ptr<Component>> components_;
};

std::ostream &operator<<(std::ostream& out, const Circuit& circuit);