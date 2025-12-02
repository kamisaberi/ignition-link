#include <ignlink/msg/node.h>
#include <iostream> // For now

// This is the private implementation
struct ignlink::msg::Node::Impl {
    std::string name;
    // Later, this will hold a shared pointer to a NodeContext,
    // which manages the actual transport layers (IPC, UDP).
};

ignlink::msg::Node::Node(const std::string& node_name)
    : pimpl_(std::make_unique<Impl>()) {
    pimpl_->name = node_name;
    std::cout << "Node '" << pimpl_->name << "' created." << std::endl;
}

ignlink::msg::Node::~Node() {
    std::cout << "Node '" << pimpl_->name << "' destroyed." << std::endl;
}

const std::string& ignlink::msg::Node::get_name() const {
    return pimpl_->name;
}