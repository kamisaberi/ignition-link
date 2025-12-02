#include "publisher_impl.h"
#include "node_context.h" // Now we can include the full definition
#include <ignlink/core/logger.h>

namespace ignlink {
namespace msg {

PublisherImpl::PublisherImpl(const std::string& topic_name,
                             const std::string& type_name,
                             std::weak_ptr<NodeContext> context)
    : topic_name_(topic_name), type_name_(type_name), context_(context) {}

core::Status PublisherImpl::publish(const std::any& msg) {
    // Check if the context (the engine room) is still alive.
    if (context_.expired()) {
        core::Logger::warn("Attempted to publish on topic '{}' but NodeContext is gone.", topic_name_);
        return core::Status(core::Status::Code::Unavailable, "NodeContext has been destroyed.");
    }
    
    // In a real implementation, this is where you would:
    // 1. Lock the context_ shared_ptr.
    // 2. Serialize the message (the `std::any`) into a byte buffer.
    // 3. Get the transport layer from the context.
    // 4. Call `transport->write(buffer)`.
    
    core::Logger::trace("Publishing message on topic '{}'", topic_name_);
    
    // For now, our job is just to log that we were called.
    return core::Status::OK();
}

} // namespace msg
} // namespace ignlink