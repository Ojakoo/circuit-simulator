#pragma once

#include "active_component.hpp"


class CurrentSource : public ActiveComponent {
    public:
        CurrentSource(const std::string& name,
                        float current = 0,
                        std::shared_ptr<Node> input = nullptr,
                        std::shared_ptr<Node> output = nullptr);

        float GetValue() const;

        void SetValue(float newval);

        ComponentType GetType() const;

    private:
        float current_;  // amperes
};