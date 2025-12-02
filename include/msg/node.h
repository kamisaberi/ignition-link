#pragma once

#include <ignlink/core/types.h>    // For Status, Tensor, etc.
#include <ignlink/msg/publisher.h>
#include <ignlink/msg/subscriber.h>

#include <memory>
#include <string>
#include <functional>

namespace ignlink {
namespace msg {

// Forward declaration to break circular dependency if NodeContext is complex.
class NodeContext;

/**
 * @class Node
 * @brief The fundamental building block for an Ignition Link application.
 *
 * A Node is an executable unit that can communicate with other nodes over the
 * Ignition Link messaging bus. It acts as a factory for creating Publishers
 * and Subscribers. Each application is typically composed of one or more Nodes,
 * each responsible for a specific task (e.g., a "camera_node", a "perception_node").
 */
class Node {
public:
    /**
     * @brief Constructs a new Node.
     * @param node_name The unique name for this node. This name is used for
     *                  identification and logging within the system.
     */
    explicit Node(const std::string& node_name);

    /**
     * @brief Destructor. Cleans up all associated publishers and subscribers.
     */
    ~Node();

    /**
     * @brief Creates a Publisher to broadcast messages on a specific topic.
     *
     * A Publisher allows this node to send messages of a given type to any
     * Subscribers listening on the same topic.
     *
     * @tparam T The C++ type of the message to be published (e.g., ignlink::core::Tensor).
     * @param topic_name The name of the topic to publish on (e.g., "/camera/image_raw").
     * @return A std::shared_ptr to the created Publisher. Returns nullptr on failure.
     *
     * @example
     *   auto image_pub = my_node.create_publisher<ignlink::core::Tensor>("/camera/image_raw");
     *   if (image_pub) {
     *       image_pub->publish(my_image_tensor);
     *   }
     */
    template <typename T>
    std::shared_ptr<Publisher<T>> create_publisher(const std::string& topic_name) {
        // In the beginning, the implementation is just a placeholder.
        // The real logic will be in the NodeContext.
        // For now, this demonstrates the API.
        return std::make_shared<Publisher<T>>(topic_name /*, context_ptr */);
    }

    /**
     * @brief Creates a Subscriber to receive messages from a specific topic.
     *
     * A Subscriber listens for messages of a given type on a specific topic and
     * executes a callback function whenever a message is received.
     *
     * @tparam T The C++ type of the message to be received.
     * @param topic_name The name of the topic to subscribe to.
     * @param callback The function to execute when a message is received. The
     *                 function should take a `const T&` as its argument.
     * @return A std::shared_ptr to the created Subscriber. Returns nullptr on failure.
     *
     * @example
     *   auto image_sub = my_node.create_subscriber<ignlink::core::Tensor>(
     *       "/camera/image_raw",
     *       [](const ignlink::core::Tensor& msg) {
     *           std::cout << "Received image with timestamp " << msg.timestamp << std::endl;
     *       }
     *   );
     */
    template <typename T>
    std::shared_ptr<Subscriber<T>> create_subscriber(
        const std::string& topic_name,
        std::function<void(const T&)> callback) {
        // Placeholder implementation for the API demonstration.
        return std::make_shared<Subscriber<T>>(topic_name, callback /*, context_ptr */);
    }

    /**
     * @brief Gets the name of the node.
     * @return The node's name as a const std::string&.
     */
    const std::string& get_name() const;

private:
    // PIMPL (Pointer to Implementation) Idiom:
    // This is a key C++ pattern. It hides all the complex internal state and
    // dependencies of the Node from this public header file. This keeps your
    // public API clean and reduces compile times.
    struct Impl;
    std::unique_ptr<Impl> pimpl_;
};

} // namespace msg
} // namespace ignlink