#pragma once

#include <list>
#include <iostream>
#include <map>
#include <algorithm>

#include "component.hpp"
#include "node.hpp"
#include "Eigen/Dense"

using namespace Eigen;

typedef std::complex<float> cd;

class Circuit {
public:
    Circuit() {}

    const std::list<std::shared_ptr<Component>>& GetComponents() const;

    const float GetOmega() const { return omega_; };
    const MatrixXcf GetAMatrix() const;
    const VectorXf GetZMatrix() const;    
 
    const std::map<std::string, int> GetNodeIndexes() const;
    const std::map<std::string, int> GetVoltageSourceIndexes() const;
    const std::map<std::string, int> GetInductorIndexes() const { return inductor_indexes_; };

    void SetOmega( float omega ) { omega_ = omega; };

    const std::shared_ptr<Node> AddNode(const std::string& node_name);
    const std::shared_ptr<Node> AddNode();
    void RemoveNode(const std::string& node_name);
    void RemoveUnnecessaryNodes();
    void AddComponent(std::shared_ptr<Component> component);
    void ConstructMatrices();
    void RemoveComponent(std::shared_ptr<Component> component);
    std::map<std::string, std::shared_ptr<Node>> GetNodes();

private:
    MatrixXcf A_;
    VectorXf z_;

    float omega_ = 0;

    std::map<std::string, std::shared_ptr<Node>> nodes_;
    std::list<std::shared_ptr<Component>> components_;

    std::map<std::string, int> node_indexes_;
    std::map<std::string, int> voltage_source_indexes_;
    std::map<std::string, int> inductor_indexes_;
    std::map<std::string, float> node_voltages_;
    std::map<std::string, float> voltage_source_currents_;
    std::map<std::string, float> inductor_currents_;
};

std::ostream &operator<<(std::ostream& out, const Circuit& circuit);
