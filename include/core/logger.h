#pragma once

#include <string>
#include <memory>

// We forward-declare the spdlog::logger to avoid including the heavy spdlog
// header in our public API. This is a good practice (PIMPL idiom).
namespace spdlog {
class logger;
}

namespace ignlink {
namespace core {

/**
 * @class Logger
 * @brief A simple, professional logging interface for the Ignition Link ecosystem.
 *
 * This class provides a set of static methods for system-wide logging. It allows
 * for setting a log level and directing output. Under the hood, it is a wrapper
 * around a powerful logging library like spdlog.
 */
class Logger {
public:
    enum class Level {
        Trace,
        Debug,
        Info,
        Warn,
        Error,
        Critical
    };

    /**
     * @brief Initializes the global logger with a specific name and level.
     * This should be called once at the start of an application.
     * @param logger_name The name for the logger (e.g., the node name).
     * @param level The minimum level of messages to log.
     */
    static void init(const std::string& logger_name, Level level = Level::Info);

    /**
     * @brief Logs a trace-level message.
     * @tparam... Args Variadic arguments for the format string.
     * @param fmt The format string (fmt::format style).
     * @param... args The arguments for the format string.
     */
    template <typename... Args>
    static void trace(const std::string& fmt, Args&&... args);

    /**
     * @brief Logs an info-level message.
     */
    template <typename... Args>
    static void info(const std::string& fmt, Args&&... args);
    
    /**
     * @brief Logs a warning-level message.
     */
    template <typename... Args>
    static void warn(const std::string& fmt, Args&&... args);

    /**
     * @brief Logs an error-level message.
     */
    template <typename... Args>
    static void error(const std::string& fmt, Args&&... args);

private:
    // The static pointer to the actual logger implementation.
    static std::shared_ptr<spdlog::logger> s_logger_;
};

} // namespace core
} // namespace ignlink