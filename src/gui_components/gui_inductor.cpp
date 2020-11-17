#include "gui_components/gui_component.hpp"
#include "gui_components/gui_inductor.hpp"


const std::string GUIInductor::INDUCTOR_TEXTURE = "../sprites/inductor.png";

GUIInductor::GUIInductor(const std::string& name,
                        float inductance,
                        std::shared_ptr<Node> input,
                        std::shared_ptr<Node> output,
                        const std::string& texture)
    : GUIComponent(texture, std::make_shared<Inductor>(name, inductance, input, output)) { }