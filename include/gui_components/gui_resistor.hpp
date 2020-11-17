#pragma once

#include "gui_components/gui_component.hpp"

#include "resistor.hpp"


class GUIResistor : public GUIComponent {
    public:
        static const std::string RESISTOR_TEXTURE;

        GUIResistor(const std::string& name,
                    float resistance = 0,
                    std::shared_ptr<Node> input = nullptr,
                    std::shared_ptr<Node> output = nullptr,
                    const std::string& texture = RESISTOR_TEXTURE
                    );
};