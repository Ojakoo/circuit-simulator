#pragma once

#include "gui_components/gui_component.hpp"

#include "inductor.hpp"


class GUIInductor : public GUIComponent {
    public:
        static const std::string INDUCTOR_TEXTURE;

        GUIInductor(const std::string& name,
                    float inductance = 0,
                    std::shared_ptr<Node> input = nullptr,
                    std::shared_ptr<Node> output = nullptr,
                    const std::string& texture = INDUCTOR_TEXTURE
                    );
};