#include "gui_components/gui_component.hpp"
#include "gui_components/gui_capacitor.hpp"


const std::string GUICapacitor::CAPACITOR_TEXTURE = "../sprites/capacitor.png";

GUICapacitor::GUICapacitor(const std::string& name,
                        float capacitance,
                        std::shared_ptr<Node> input,
                        std::shared_ptr<Node> output,
                        const std::string& texture)
    : GUIComponent(texture, std::make_shared<Capacitor>(name, capacitance, input, output)) { }