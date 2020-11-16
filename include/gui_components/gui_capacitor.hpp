#pragma once

#include "gui_components/gui_component.hpp"

#include "capacitor.hpp"


class GUICapacitor : public GUIComponent, public Capacitor {
    public:
        static const std::string CAPACITOR_TEXTURE;

        GUICapacitor(const std::string& name,
                    float capacitance = 0,
                    std::shared_ptr<Node> input = nullptr,
                    std::shared_ptr<Node> output = nullptr,
                    const std::string& texture = CAPACITOR_TEXTURE
                    );
};