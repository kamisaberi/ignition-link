#pragma once

#include <ignlink/core/status.h>
#include <ignlink/core/types.h> // For Tensor, etc.

#include <string>
#include <memory>
#include <any>

namespace ignlink {
namespace msg {

// Forward declare to avoid circular includes
class NodeContext;

/**
 * @class PublisherImpl
 * @brief (Internal) The concrete implementation of a publisher.
 *
 * This class holds the state for a publisher and contains the logic to serialize
 * a message and hand it off to the transport layer via the NodeContext.
 */
class PublisherImpl {
public:
    PublisherImpl(const std::string& topic_name,
                  const std::string& type_name,
                  std::weak_ptr<NodeContext> context);

    /**
     * @brief The core publish method called by the public Publisher handle.
     * @param msg The message to publish, passed as a std::any to be type-erased.
     * @return Status indicating success or failure.
     */
    core::Status publish(const std::any& msg);

    const std::string& get_topic_name() const { return topic_name_; }
    const std::string& get_type_name() const { return type_name_; }

private:
    std::string topic_name_;
    std::string type_name_;
    std::weak_ptr<NodeContext> context_; // Use a weak_ptr to avoid circular ownership
};

} // namespace msg
} // namespace ignlink