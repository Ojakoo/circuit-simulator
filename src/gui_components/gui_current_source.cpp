#include "gui_components/gui_component.hpp"
#include "gui_components/gui_current_source.hpp"


const std::string GUICurrentSource::C_SOURCE_TEXTURE = "../sprites/current_source.png";

GUICurrentSource::GUICurrentSource(const std::string& name,
                        float current,
                        std::shared_ptr<Node> input,
                        std::shared_ptr<Node> output,
                        const std::string& texture)
    : GUIComponent(texture, std::make_shared<CurrentSource>(name, current, input, output)) { }