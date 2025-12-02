#pragma once

#include <string>
#include <memory>
#include <functional>

namespace ignlink {
namespace msg {

// Forward declare the implementation details.
class SubscriberImpl;

/**
 * @class Subscriber
 * @brief A handle for receiving messages of a specific type from a topic.
 *
 * A Subscriber is created by a Node and is strongly typed. It listens for messages
 * on a specific topic and executes a user-provided callback function for each
 * message received. This class is a lightweight handle.
 *
 * @tparam T The C++ type of the message to be received.
 */
template <typename T>
class Subscriber {
public:
    /**
     * @brief Gets the name of the topic this subscriber is listening to.
     * @return The topic name as a const std::string&.
     */
    const std::string& get_topic_name() const;

private:
    // This class is not meant to be constructed directly by the user, only by a Node.
    friend class Node;

    /**
     * @brief Private constructor.
     * @param topic_name The name of the topic.
     * @param callback The user's callback function.
     * @param impl A shared pointer to the underlying implementation object.
     */
    Subscriber(const std::string& topic_name,
               std::function<void(const T&)> callback,
               std::shared_ptr<SubscriberImpl> impl);
    
    std::string topic_name_;
    std::function<void(const T&)> callback_;
    std::shared_ptr<SubscriberImpl> pimpl_;
};

} // namespace msg
} // namespace ignlink