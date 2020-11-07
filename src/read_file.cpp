#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <exception>
#include <memory>

#include "circuit.hpp"
#include "component.hpp"
#include "resistor.hpp"
#include "inductor.hpp"
#include "capacitor.hpp"
#include "dc_voltage_source.hpp"
#include "dc_current_source.hpp"
#include "node.hpp"
#include "read_file.hpp"


Circuit ReadCircuitFromFile(const std::string& file_name) {

    /*
    Netlist format:
    Type, Name, Input, Output, Value

    For Example:

    V V1 0 N001 5
    R R1 N001 N002 10
    C C1 N002 0 0.001
    L L1 0 N002 0.05

    */
    Circuit circuit;
    std::ifstream ifstr(file_name);

    if (ifstr.rdstate() & (ifstr.failbit | ifstr.badbit)) {
        std::cerr << "Failed" << std::endl;
    } else {
        while ( !ifstr.eof() ) {
            // read a line from file
            std::string line;
            std::getline(ifstr, line);
            std::stringstream iss(line);

            std::string type;
            std::string name;
            std::string input_node;
            std::string output_node;
            float value;

            iss >> type >> name >> input_node >> output_node >> value;

            std::shared_ptr<Node> in = circuit.AddNode(input_node);
            std::shared_ptr<Node> out = circuit.AddNode(output_node);
            
            iss >> type >> name >> input_node >> output_node >> value;
            if ( type == "R" ) {
                std::shared_ptr<Resistor> R = std::make_shared<Resistor>(name, value, in, out);
                circuit.AddComponent(R);
            } else if ( type == "L" ) {
                std::shared_ptr<Inductor> L = std::make_shared<Inductor>(name, value, in, out);
                circuit.AddComponent(L);
            } else if ( type == "C" ) {
                std::shared_ptr<Capacitor> C = std::make_shared<Capacitor>(name, value, in, out);
                circuit.AddComponent(C);
            } else if ( type == "V" ) {
                std::shared_ptr<DCVoltageSource> V = std::make_shared<DCVoltageSource>(name, value, in, out);
                circuit.AddComponent(V);
            } else {
                throw std::invalid_argument("Invalid netlist.");
            }
        }
    }
    return circuit;
}