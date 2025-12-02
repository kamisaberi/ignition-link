#include "subscriber_impl.h"
#include <ignlink/core/logger.h>

namespace ignlink {
namespace msg {

SubscriberImpl::SubscriberImpl(const std::string& topic_name,
                               const std::string& type_name,
                               std::function<void(const std::any&)> callback,
                               std::weak_ptr<NodeContext> context)
    : topic_name_(topic_name), type_name_(type_name), callback_(callback), context_(context) {}

void SubscriberImpl::invoke_callback(const std::any& msg) {
    if (callback_) {
        core::Logger::trace("Invoking callback for topic '{}'", topic_name_);
        callback_(msg);
    }
}

} // namespace msg
} // namespace ignlink