#pragma once

#include <string>
#include <memory>

namespace ignlink {
namespace msg {

// Forward declare the implementation details.
class PublisherImpl;

/**
 * @class Publisher
 * @brief A handle for publishing messages of a specific type to a topic.
 *
 * A Publisher is created by a Node and is strongly typed. It can only publish
 * messages of the type specified during its creation. This class is a lightweight
 * handle; the heavy lifting is done by the underlying implementation.
 *
 * @tparam T The C++ type of the message to be published.
 */
template <typename T>
class Publisher {
public:
    /**
     * @brief Publishes a message to the topic.
     *
     * This method sends a copy of the message to all subscribers listening on this
     * publisher's topic. The actual sending mechanism (e.g., shared memory) is
     * handled by the underlying transport layer.
     *
     * @param msg The message to be published. It is passed by const reference
     *            for efficiency.
     */
    void publish(const T& msg);

    /**
     * @brief Gets the name of the topic this publisher is associated with.
     * @return The topic name as a const std::string&.
     */
    const std::string& get_topic_name() const;

private:
    // This class is not meant to be constructed directly by the user, only by a Node.
    // The `friend` declaration allows the Node class to access our private constructor.
    friend class Node;

    /**
     * @brief Private constructor.
     * @param topic_name The name of the topic.
     * @param impl A shared pointer to the underlying implementation object, which
     *             is provided by the Node's context.
     */
    Publisher(const std::string& topic_name, std::shared_ptr<PublisherImpl> impl);

    std::string topic_name_;
    std::shared_ptr<PublisherImpl> pimpl_;
};

} // namespace msg
} // namespace ignlink