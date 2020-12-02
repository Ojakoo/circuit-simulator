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
#include "voltage_source.hpp"
#include "current_source.hpp"
#include "node.hpp"
#include "save_and_load.hpp"

void SaveNetList(Circuit& circuit, const std::string& file_name) {

    // open a file for writing
    std::ofstream os(file_name);

    // write to the file
    auto begin = circuit.GetComponents().begin();
	for ( ; begin != circuit.GetComponents().end(); begin++ ) {
        ComponentType type = (*begin)->GetType();
        std::string symbol;
        switch ( type ) {
            case RESISTOR:
                symbol = "R";
                break;
            case CAPACITOR:
                symbol = "C";
                break;
            case INDUCTOR:
                symbol = "L";
                break;
            case VOLTAGE_SOURCE:
                symbol = "V";
                break;
            case CURRENT_SOURCE:
                symbol = "J";
                break;
        }
        os << symbol << " "
           << (*begin)->GetName() << " "
           << (*begin)->GetTerminalNode(INPUT)->GetName() << " "
           << (*begin)->GetTerminalNode(OUTPUT)->GetName() << " "
           << (*begin)->GetValue() ;
        
        // add newline if not last component
        if (begin != --circuit.GetComponents().end()) os << std::endl;
    }

	// close the file
	os.close();
}

Circuit LoadNetList(const std::string& file_name) {

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
        throw std::runtime_error("Failed to read netlist file.");
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

            if (!iss) {
                // Check that reading succeeded
                throw std::runtime_error("Error while reading netlist file.");
            }

            std::shared_ptr<Node> in = circuit.AddNode(input_node);
            std::shared_ptr<Node> out = circuit.AddNode(output_node);
        
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
                std::shared_ptr<VoltageSource> V = std::make_shared<VoltageSource>(name, value, in, out);
                circuit.AddComponent(V);
            } else {
                throw std::runtime_error("Invalid component type found in netlist.");
            }
        }
    }

    ifstr.close();
    return circuit;
}