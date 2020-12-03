#include "gui_components/gui_component.hpp"
#include "gui_components/gui_voltage_source.hpp"


const std::string GUIVoltageSource::V_SOURCE_TEXTURE = "../sprites/voltage_source.png";

GUIVoltageSource::GUIVoltageSource(const std::string& name,
                        float voltage,
                        std::shared_ptr<Node> input,
                        std::shared_ptr<Node> output,
                        const std::string& texture)
    : GUIComponent(texture, std::make_shared<VoltageSource>(name, voltage, input, output)) { }