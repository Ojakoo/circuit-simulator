#include "node.hpp"


Node::Node(const std::string &name, NodeType type)
    : name_(name), type_(type) { }

const std::string& Node::GetName() const {
    return name_;
}

void Node::SetNodeType(NodeType type) {
    type_ = type;
}