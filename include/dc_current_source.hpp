#pragma once

#include "component.hpp"


class DCCurrentSource : public Component{
    public:
        DCCurrentSource(const std::string& name,
                        float current = 0,
                        std::shared_ptr<Node> input = nullptr,
                        std::shared_ptr<Node> output = nullptr);

        float GetValue() const;

        ComponentType GetType() const;

    private:
        float current_;  // amperes
};