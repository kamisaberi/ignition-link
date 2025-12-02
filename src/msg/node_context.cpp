#include "node_context.h" // Internal header
#include <ignlink/core/logger.h>
#include <chrono>

namespace ignlink {
namespace msg {

NodeContext::NodeContext() : running_(true) {
    // Start the background thread when the context is created.
    // The `spin` function will be the entry point for this thread.
    spin_thread_ = std::thread(&NodeContext::spin, this);
    core::Logger::info("NodeContext started spin thread.");
}

NodeContext::~NodeContext() {
    // Signal the spin thread to stop and wait for it to finish.
    running_.store(false);
    if (spin_thread_.joinable()) {
        spin_thread_.join();
    }
    core::Logger::info("NodeContext stopped spin thread.");
}

core::Status NodeContext::register_publisher(std::shared_ptr<PublisherImpl> impl) {
    std::lock_guard<std::mutex> lock(pub_sub_mutex_);
    // In a real implementation, you would check for name collisions etc.
    // For now, we just add it to our map.
    // publishers_[impl->get_topic_name()].push_back(impl);
    core::Logger::info("Registered publisher for topic '{}'", "some_topic"); // Placeholder
    return core::Status::OK();
}

core::Status NodeContext::register_subscriber(std::shared_ptr<SubscriberImpl> impl) {
    std::lock_guard<std::mutex> lock(pub_sub_mutex_);
    // subscribers_[impl->get_topic_name()].push_back(impl);
    core::Logger::info("Registered subscriber for topic '{}'", "some_topic"); // Placeholder
    return core::Status::OK();
}

void NodeContext::spin() {
    while (running_.load()) {
        // This is the core loop of the messaging system.
        // In a real implementation, this is where you would:
        
        // 1. Poll the transport layer for new incoming messages.
        //    auto new_message = transport_->read(timeout);
        
        // 2. If a message is received:
        //    - Deserialize it to find its topic name and data.
        //    - Look up the subscribers for that topic in the `subscribers_` map.
        //    - For each subscriber, add its callback to a thread pool to be executed.
        
        // For now, we'll just sleep to simulate work and prevent a tight loop.
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

} // namespace msg
} // namespace ignlink