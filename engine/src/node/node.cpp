#include "engine/nodes/node.h"

namespace engine {

const std::vector<std::shared_ptr<Node>>& Node::GetChildren() const {
    return _children;
}

void Node::AddChild(const std::shared_ptr<Node>& child) {
    child->_parent = shared_from_this();
    _children.push_back(child);
}

void Node::RemoveChild(const std::shared_ptr<Node>& child) {
    child->_parent.reset();
    std::erase(_children, child);
}

void Node::Update() {
    UpdateSelf();
    for (const auto& child : _children)
        child->Update();
}

}