#include "node.hpp"


Node::Node(const std::string &name, NodeType type)
    : name_(name), type_(type) { }

const std::string& Node::GetName() const {
    return name_;
}

const NodeType Node::GetType() const {
    return type_;
}

void Node::SetNodeType(NodeType type) {
    type_ = type;
}

std::ostream &operator<<(std::ostream& out, const Node& node) {
    out << "\n" << node.GetName();
    if (node.GetType() == GROUND) {
        out << " Type: Ground";
    } else {
        out << " Type: Normal";
    }
    return out.flush();
}