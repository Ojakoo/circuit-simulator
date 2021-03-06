#include "active_component.hpp"


ActiveComponent::ActiveComponent(const std::string &name,
                                 std::shared_ptr<Node> input,
                                 std::shared_ptr<Node> output,
                                 float freq)
    : Component(name, input, output), freq_(freq) { }


ComponentClass ActiveComponent::GetClass() const {
    return ACTIVE;
};