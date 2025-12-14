#pragma once
#include <memory>
#include <vector>

namespace engine {

class Node : public std::enable_shared_from_this<Node> {
    std::weak_ptr<Node> _parent;
    std::vector<std::shared_ptr<Node>> _children{};
public:
    virtual ~Node() = default;
    Node() = default;

    const std::vector<std::shared_ptr<Node>>& GetChildren() const;
    void AddChild(const std::shared_ptr<Node>& child);
    void RemoveChild(const std::shared_ptr<Node>& child);
    void Update();
protected:
    virtual void UpdateSelf() {}
};

}
