#pragma once

#include "gui_components/gui_component.hpp"

#include "voltage_source.hpp"


class GUIVoltageSource : public GUIComponent {
    public:
        static const std::string V_SOURCE_TEXTURE;

        GUIVoltageSource(const std::string& name,
                    float voltage = 0,
                    std::shared_ptr<Node> input = nullptr,
                    std::shared_ptr<Node> output = nullptr,
                    const std::string& texture = V_SOURCE_TEXTURE
                    );
};