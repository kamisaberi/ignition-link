#pragma once

#include <ignlink/core/status.h>
#include <ignlink/msg/publisher.h>   // For declaration
#include <ignlink/msg/subscriber.h>  // For declaration

#include <memory>
#include <string>
#include <vector>
#include <map>
#include <mutex>
#include <thread>
#include <atomic>

namespace ignlink {
namespace msg {

// Forward declarations of internal implementation classes
class PublisherImpl;
class SubscriberImpl;

/**
 * @class NodeContext
 * @brief (Internal) The central engine for the messaging system within a process.
 *
 * This class is the hidden workhorse. It manages a background thread to handle
 * message dispatching, maintains registries of all publishers and subscribers,
 * and interfaces with the underlying transport layer (e.g., shared memory).
 * There is typically only one NodeContext per process.
 */
class NodeContext {
public:
    NodeContext();
    ~NodeContext();

    // Prevent copying
    NodeContext(const NodeContext&) = delete;
    NodeContext& operator=(const NodeContext&) = delete;

    /**
     * @brief Registers a new publisher with the context.
     * @param impl The publisher implementation to register.
     * @return Status indicating success or failure.
     */
    core::Status register_publisher(std::shared_ptr<PublisherImpl> impl);

    /**
     * @brief Registers a new subscriber with the context.
     * @param impl The subscriber implementation to register.
     * @return Status indicating success or failure.
     */
    core::Status register_subscriber(std::shared_ptr<SubscriberImpl> impl);
    
    // ... (methods for unregistering would also be here)

private:
    /**
     * @brief The main loop for the background thread.
     *
     * This function runs continuously, polling the transport layer for new
     * messages and dispatching them to the appropriate subscriber callbacks.
     */
    void spin();

    std::mutex pub_sub_mutex_; // Mutex to protect access to the registries

    // Registries for all publishers and subscribers in this process
    std::map<std::string, std::vector<std::shared_ptr<PublisherImpl>>> publishers_;
    std::map<std::string, std::vector<std::shared_ptr<SubscriberImpl>>> subscribers_;

    std::atomic<bool> running_; // Atomic flag to control the spin thread
    std::thread spin_thread_;   // The background thread for message dispatching
};

} // namespace msg
} // namespace ignlink