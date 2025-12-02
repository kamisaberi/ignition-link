#pragma once

#include <string>
#include <memory>
#include <functional>
#include <any>

namespace ignlink {
namespace msg {

class NodeContext;

/**
 * @class SubscriberImpl
 * @brief (Internal) The concrete implementation of a subscriber.
 *
 * This class holds the state for a subscriber, including its type-erased
 * callback. It is invoked by the NodeContext's spin thread when a new message arrives.
 */
class SubscriberImpl {
public:
    SubscriberImpl(const std::string& topic_name,
                   const std::string& type_name,
                   std::function<void(const std::any&)> callback,
                   std::weak_ptr<NodeContext> context);
    
    /**
     * @brief The core method to invoke the user's callback with a received message.
     * @param msg The received message, passed as a type-erased std::any.
     */
    void invoke_callback(const std::any& msg);

    const std::string& get_topic_name() const { return topic_name_; }
    const std::string& get_type_name() const { return type_name_; }

private:
    std::string topic_name_;
    std::string type_name_;
    std::function<void(const std::any&)> callback_; // Type-erased callback
    std::weak_ptr<NodeContext> context_;
};

} // namespace msg
} // namespace ignlink