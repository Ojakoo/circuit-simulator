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
    const int GetNodeCount() const;
    const int GetSourceCount() const;
    const int GetVoltageSourceCount() const;
    const std::map<std::string, int> GetNodeIndexes() const;
    const std::map<std::string, int> GetVoltageSourceIndexes() const;
    const std::shared_ptr<Node> AddNode(const std::string& node_name);
    void AddComponent(std::shared_ptr<Component> component);
    void ConstructMatrices();
    const MatrixXcf GetAMatrix() const;
    const VectorXf GetZMatrix() const;
    void RemoveComponent(std::shared_ptr<Component> component);
private:
    int i_ = 0; // number of sources
    int n_ = 0; // number of nodes (excluding ground node)
    int m_ = 0; // number of voltage sources
    MatrixXcf A_;
    VectorXf z_;
    std::map<std::string, std::shared_ptr<Node>> nodes_;
    std::map<std::string, int> node_indexes_;
    std::map<std::string, int> voltage_source_indexes_;
    std::map<std::string, float> node_voltages_;
    std::map<std::string, float> voltage_source_currents_;
    std::list<std::shared_ptr<Component>> components_;
};

std::ostream &operator<<(std::ostream& out, const Circuit& circuit);
