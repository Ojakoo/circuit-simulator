#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
// #include <exception>
// #include <memory>

#include "component.hpp"
#include "resistor.hpp"
#include "inductor.hpp"
#include "capacitor.hpp"
#include "dc_voltage_source.hpp"
#include "dc_current_source.hpp"
#include "node.hpp"
#include "read_file.hpp"

/*
Component CreateComponent(std::stringstream iss) {
    std::string type;  // component type (R, L, C, V, J)
    std::string name;  // component name
    std::string input_node;  // negative
    std::string output_node;  // positive
    float value;  // value (eg. resistance, capacitance)
    
    iss >> type >> name >> input_node >> output_node >> value;
    if ( type == "R" ) {
        Component c = Resistor(name, value);
    } else if ( type == "L" ) {
        Component c = Inductor(name, value);
    } else if ( type == "C" ) {
        Component c = Capacitor(name, value);
    } else if ( type == "V" ) {
        Component c = DCVoltageSource(name, value);
    } else {
        throw std::invalid_argument("Invalid netlist.");
    }
    std::shared_ptr<Node> in = std::make_shared<Node>(
        input_node, input_node == "0" ? GROUND : NORMAL);
    std::shared_ptr<Node> out = std::make_shared<Node>(
        input_node, input_node == "0" ? GROUND : NORMAL);
}
*/

void ReadCircuitFromFile(std::string& file_name) {

    /*
    Netlist format:
    Type, Name, Input, Output, Value

    For Example:

    V V1 0 N001 5
    R R1 N001 N002 10
    C C1 N002 0 0.001
    L L1 0 N002 0.05

    */

    std::ifstream ifstr(file_name);
    if (ifstr.rdstate() & (ifstr.failbit | ifstr.badbit)) {
        std::cerr << "Failed" << std::endl;
    } else {
        while ( !ifstr.eof() ) {
            // read a line from file
            std::string line;
            std::getline(ifstr, line);
            std::stringstream iss(line);
            
            std::cout << iss.str() << std::endl;
        }
    }
}