#pragma once
#include <memory>
#include <vector>

namespace engine {

class Node {
    std::shared_ptr<Node> _parent;
    std::vector<std::shared_ptr<Node>> _children;
public:
    explicit Node(const std::shared_ptr<Node>& parent):
        _parent(parent) {}

    const std::vector<std::shared_ptr<Node>>& GetChildren() const {
        return _children;
    }

    void AddChild(const std::shared_ptr<Node>& child) {
        _children.push_back(child);
    }

    void RemoveChild(const std::shared_ptr<Node>& child) {
        std::erase(_children, child);
    }
public:
    void Update() {
        UpdateSelf();
        for (auto& child : _children)
            child->Update();
    }
protected:
    virtual void UpdateSelf() {}
};

}
