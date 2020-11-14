#include "gui_components/gui_component.hpp"
#include "gui_components/gui_resistor.hpp"


const std::string GUIResistor::RESISTOR_TEXTURE = "../sprites/resistor.png";

GUIResistor::GUIResistor(const std::string& name,
                        float resistance,
                        std::shared_ptr<Node> input,
                        std::shared_ptr<Node> output,
                        const std::string& texture)
    : GUIComponent(texture),
      Resistor(name, resistance, input, output) { }