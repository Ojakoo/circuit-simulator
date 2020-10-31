#pragma once

#include "component.hpp"


class DCCurrentSource : public Component{
    public:
        DCCurrentSource(const std::string& name, float current);

        float GetValue() const;

        ComponentType GetType() const;

    private:
        float current_;  // amperes
};