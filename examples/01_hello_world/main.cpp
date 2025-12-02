#include <ignlink/msg/node.h>
#include <iostream>

int main() {
    // 1. Create a node
    ignlink::msg::Node my_node("my_first_node");
    std::cout << "Hello from " << my_node.get_name() << "!" << std::endl;

    // 2. Create a publisher (it won't do anything yet)
    auto chatter_pub = my_node.create_publisher<std::string>("/chatter");
    
    // 3. Create a subscriber (it won't receive anything yet)
    auto chatter_sub = my_node.create_subscriber<std::string>(
        "/chatter",
        [](const std::string& msg) { /* callback */ }
    );

    std::cout << "Successfully created a publisher and subscriber." << std::endl;
    // The node will be destroyed when it goes out of scope.

    return 0;
}