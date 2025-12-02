#pragma once

#include "node_context.h"
#include <memory>

namespace ignlink {
namespace msg {

/**
 * @class ContextManager
 * @brief (Internal) A singleton to manage the global NodeContext instance.
 */
class ContextManager {
public:
    /**
     * @brief Gets the singleton instance of the NodeContext.
     * @return A shared_ptr to the global NodeContext.
     */
    static std::shared_ptr<NodeContext> get_instance();
};

} // namespace msg
} // namespace ignlink