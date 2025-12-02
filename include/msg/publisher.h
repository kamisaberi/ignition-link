#pragma once
#include <string>
namespace ignlink { namespace msg {
    template <typename T>
    class Publisher {
    public:
        Publisher(const std::string& topic_name) {}
        void publish(const T& msg) { /* no-op */ }
    };
}}