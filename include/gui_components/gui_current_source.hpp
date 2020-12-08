#pragma once

#include "gui_components/gui_component.hpp"

#include "current_source.hpp"


class GUICurrentSource : public GUIComponent {
    public:
        static const std::string C_SOURCE_TEXTURE;

        GUICurrentSource(const std::string& name,
                    float current = 0.0,
                    std::shared_ptr<Node> input = nullptr,
                    std::shared_ptr<Node> output = nullptr,
                    const std::string& texture = C_SOURCE_TEXTURE
                    );
};