#pragma once

#include <list>
#include <iostream>
#include <map>
#include <algorithm>

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

    const float GetOmega() const { return omega_; };
    const MatrixXcf GetAMatrix() const;
    const VectorXf GetZMatrix() const;    
 
    const std::map<std::string, int> GetNodeIndexes() const;
    const std::map<std::string, int> GetVoltageSourceIndexes() const;

    void SetOmega( float omega ) { omega_ = omega; };

    const std::shared_ptr<Node> AddNode(const std::string& node_name);

    void AddComponent(std::shared_ptr<Component> component);
    void ConstructMatrices();
    void ConstructMatrices2();
    void StampLogic( float omega, int inductor_count );
    void RemoveComponent(std::shared_ptr<Component> component);

private:
    int i_ = 0; // number of sources
    int n_ = 0; // number of nodes (excluding ground node)
    int m_ = 0; // number of voltage sources
    float omega_ = 0; // default omega

    MatrixXcf A_;
    VectorXf z_;

    std::map<std::string, std::shared_ptr<Node>> nodes_;
    std::list<std::shared_ptr<Component>> components_;

    std::map<std::string, int> node_indexes_;
    std::map<std::string, int> voltage_source_indexes_;
    std::map<std::string, int> inductor_sc_indexes_;
    std::map<std::string, float> node_voltages_;
    std::map<std::string, float> voltage_source_currents_;
    std::map<std::string, float> inductor_sc_currents_;
};

std::ostream &operator<<(std::ostream& out, const Circuit& circuit);
