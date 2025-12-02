#include "context_manager.h"
#include <mutex>

namespace ignlink {
namespace msg {

// The static pointer to the singleton instance.
static std::shared_ptr<NodeContext> g_context = nullptr;
static std::mutex g_context_mutex;

std::shared_ptr<NodeContext> ContextManager::get_instance() {
    std::lock_guard<std::mutex> lock(g_context_mutex);
    if (!g_context) {
        // If it's the first time being called, create the NodeContext.
        g_context = std::make_shared<NodeContext>();
    }
    return g_context;
}

} // namespace msg
} // namespace ignlink