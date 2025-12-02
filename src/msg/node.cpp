#include <ignlink/msg/node.h>
#include <ignlink/core/logger.h>
#include <ignlink/core/status.h>

// --- Include all necessary internal headers ---
// These are private to your library's implementation.
#include "context_manager.h"
#include "node_context.h"
#include "publisher_impl.h"
#include "subscriber_impl.h"

#include <any>      // For std::any, used in type erasure
#include <typeinfo> // For getting type names

namespace ignlink {
namespace msg {

// =============================================================================
// == PRIVATE IMPLEMENTATION (PIMPL) STRUCT ====================================
// =============================================================================
// This struct contains all the private member variables for the Node class.
// It is defined here in the .cpp file to hide implementation details from
// the public header file.
struct Node::Impl {
    std::string name;
    std::shared_ptr<NodeContext> context; // A shared pointer to the central "engine room"
};

// =============================================================================
// == NODE CONSTRUCTOR, DESTRUCTOR, AND HELPERS ================================
// =============================================================================

Node::Node(const std::string& node_name)
    // Create the unique_ptr to the private implementation struct.
    : pimpl_(std::make_unique<Impl>()) {
    
    // 1. Set the node's name.
    pimpl_->name = node_name;

    // 2. Get the singleton instance of the NodeContext.
    //    This ensures all nodes in this process share the same backend.
    pimpl_->context = ContextManager::get_instance();

    // 3. Initialize the global logger with this node's name.
    //    This is useful for distinguishing logs from different nodes.
    core::Logger::init(pimpl_->name, core::Logger::Level::Info);
    core::Logger::info("Node initialized.");
}

Node::~Node() {
    core::Logger::info("Node shutting down.");
    // The unique_ptr pimpl_ is automatically destroyed here, cleaning up
    // all resources held by the Impl struct.
}

const std::string& Node::get_name() const {
    return pimpl_->name;
}

// =============================================================================
// == PUBLIC TEMPLATE DEFINITIONS (PUBLISHER & SUBSCRIBER) =====================
// =============================================================================
// The definitions for the template functions in the Node class are provided here.
// This is where the actual logic for creating publishers and subscribers lives.

template <typename T>
std::shared_ptr<Publisher<T>> Node::create_publisher(const std::string& topic_name) {
    if (!pimpl_->context) {
        core::Logger::error("Failed to create publisher for topic '{}': NodeContext is null.", topic_name);
        return nullptr;
    }

    // 1. Create the concrete implementation object. We pass it the topic name,
    //    the C++ type name (for debugging/introspection), and a weak_ptr to the context.
    auto pub_impl = std::make_shared<PublisherImpl>(topic_name, typeid(T).name(), pimpl_->context);

    // 2. Register this new implementation with the central NodeContext.
    //    The context now knows about this publisher.
    core::Status status = pimpl_->context->register_publisher(pub_impl);
    if (!status.ok()) {
        core::Logger::error("Failed to register publisher for topic '{}': {}", topic_name, status.message());
        return nullptr;
    }

    // 3. Create the public-facing "handle" (the Publisher<T> object).
    //    We use a custom constructor that takes the implementation pointer.
    //    Note: `std::make_shared` cannot access private constructors, so we use `new`.
    //    The use of a custom deleter ensures proper cleanup if needed, but a
    //    shared_ptr is generally safe.
    return std::shared_ptr<Publisher<T>>(new Publisher<T>(topic_name, pub_impl));
}

template <typename T>
std::shared_ptr<Subscriber<T>> Node::create_subscriber(
    const std::string& topic_name,
    std::function<void(const T&)> callback) {
    
    if (!pimpl_->context) {
        core::Logger::error("Failed to create subscriber for topic '{}': NodeContext is null.", topic_name);
        return nullptr;
    }

    // 1. Create a "type-erased" lambda.
    //    The underlying system (NodeContext, SubscriberImpl) only knows how to handle
    //    `std::any`. This lambda safely casts the `std::any` back to the
    //    strongly-typed `T` that the user's callback expects.
    auto type_erased_callback = [callback, topic_name](const std::any& msg) {
        try {
            // Attempt to cast the received `any` message to the type T.
            const T& typed_msg = std::any_cast<const T&>(msg);
            // If successful, call the user's original callback.
            callback(typed_msg);
        } catch (const std::bad_any_cast& e) {
            // This is a critical error, meaning a message of the wrong type
            // was received on this topic.
            core::Logger::error("Type mismatch on topic '{}'. Could not cast message. Error: {}", topic_name, e.what());
        }
    };

    // 2. Create the concrete implementation object.
    auto sub_impl = std::make_shared<SubscriberImpl>(topic_name, typeid(T).name(), type_erased_callback, pimpl_->context);
    
    // 3. Register this implementation with the central NodeContext.
    core::Status status = pimpl_->context->register_subscriber(sub_impl);
    if (!status.ok()) {
        core::Logger::error("Failed to register subscriber for topic '{}': {}", topic_name, status.message());
        return nullptr;
    }

    // 4. Create the public-facing "handle".
    return std::shared_ptr<Subscriber<T>>(new Subscriber<T>(topic_name, callback, sub_impl));
}

// =============================================================================
// == EXPLICIT TEMPLATE INSTANTIATION ==========================================
// =============================================================================
// Since the template definitions above are in a .cpp file (not a header),
// the compiler doesn't know which versions of the templates to generate.
// We must explicitly tell it which types we plan to use.
// This is a list of all message types that can be sent through the system.

template std::shared_ptr<Publisher<std::string>> Node::create_publisher(const std::string&);
template std::shared_ptr<Subscriber<std::string>> Node::create_subscriber(const std::string&, std::function<void(const std::string&)>);

// You would add more types here as you need them:
// template std::shared_ptr<Publisher<int>> Node::create_publisher(const std::string&);
// template std::shared_ptr<Subscriber<int>> Node::create_subscriber(const std::string&, std::function<void(const int&)>);
//
// template std::shared_ptr<Publisher<core::Tensor>> Node::create_publisher(const std::string&);
// template std::shared_ptr<Subscriber<core::Tensor>> Node::create_subscriber(const std::string&, std::function<void(const core::Tensor&)>);


// =============================================================================
// == FRIEND CONSTRUCTOR IMPLEMENTATIONS (for Publisher/Subscriber) ============
// =============================================================================
// The Publisher and Subscriber classes have private constructors that only the
// Node can call. The definitions for those constructors and their member
// functions must also be in a .cpp file.

// --- Publisher<T> Implementation ---
template <typename T>
Publisher<T>::Publisher(const std::string& topic_name, std::shared_ptr<PublisherImpl> impl)
    : topic_name_(topic_name), pimpl_(impl) {}

template <typename T>
void Publisher<T>::publish(const T& msg) {
    if (pimpl_) {
        // The public handle simply forwards the call to its private implementation.
        // The `std::any` conversion happens here.
        pimpl_->publish(std::any(msg));
    }
}

template <typename T>
const std::string& Publisher<T>::get_topic_name() const {
    return topic_name_;
}

// --- Subscriber<T> Implementation ---
template <typename T>
Subscriber<T>::Subscriber(const std::string& topic_name,
                          std::function<void(const T&)> callback,
                          std::shared_ptr<SubscriberImpl> impl)
    : topic_name_(topic_name), callback_(callback), pimpl_(impl) {}

template <typename T>
const std::string& Subscriber<T>::get_topic_name() const {
    return topic_name_;
}

// --- EXPLICIT INSTANTIATION FOR THE HANDLE CLASSES ---
// Just like with the Node's template methods, we need to tell the compiler
// which versions of the Publisher and Subscriber classes to generate.

template class Publisher<std::string>;
template class Subscriber<std::string>;

// You would add more types here as you need them:
// template class Publisher<int>;
// template class Subscriber<int>;
//
// template class Publisher<core::Tensor>;
// template class Subscriber<core::Tensor>;

} // namespace msg
} // namespace ignlink