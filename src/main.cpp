#include "circuit_simulator_gui.hpp"

int main ( void ) {

    CircuitSimulatorGUI window(640, 480, "Circuit Simulator");
    window.MainLoop();
    ImGui::SFML::Shutdown();
    
    return 0;

}