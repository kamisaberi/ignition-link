#include <ignlink/core/logger.h>

// This is where we include the heavy third-party library.
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace ignlink {
namespace core {

// Initialize the static logger pointer to null.
std::shared_ptr<spdlog::logger> Logger::s_logger_;

void Logger::init(const std::string& logger_name, Level level) {
    // Create a color-coded console sink.
    auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    
    // Set a standard pattern for the log messages.
    // [Timestamp] [Logger Name] [Log Level] Message
    console_sink->set_pattern("%^[%Y-%m-%d %H:%M:%S.%e] [%n] [%l] %v%$");

    s_logger_ = std::make_shared<spdlog::logger>(logger_name, console_sink);

    // Map our internal log level enum to spdlog's enum.
    spdlog::level::level_enum spdlog_level = spdlog::level::info;
    switch (level) {
        case Level::Trace:    spdlog_level = spdlog::level::trace;    break;
        case Level::Debug:    spdlog_level = spdlog::level::debug;    break;
        case Level::Info:     spdlog_level = spdlog::level::info;     break;
        case Level::Warn:     spdlog_level = spdlog::level::warn;     break;
        case Level::Error:    spdlog_level = spdlog::level::err;      break;
        case Level::Critical: spdlog_level = spdlog::level::critical; break;
    }
    s_logger_->set_level(spdlog_level);
    s_logger_->flush_on(spdlog::level::trace); // Auto-flush for all levels.
}

// The template definitions must be in the header, but if they just call another
// function, we can put that function's implementation here.
// To keep it simple, we will define the templates directly in the cpp file,
// which requires explicit instantiation. A more common way is to put the
// template implementation in a separate `-inl.h` file.

// A helper function to ensure the logger is initialized.
static void ensure_initialized() {
    if (!Logger::s_logger_) {
        // Default initialization if the user forgets to call init().
        Logger::init("IGNLINK");
    }
}

template <typename... Args>
void Logger::trace(const std::string& fmt, Args&&... args) {
    ensure_initialized();
    s_logger_->trace(fmt, std::forward<Args>(args)...);
}

template <typename... Args>
void Logger::info(const std::string& fmt, Args&&... args) {
    ensure_initialized();
    s_logger_->info(fmt, std::forward<Args>(args)...);
}

template <typename... Args>
void Logger::warn(const std::string& fmt, Args&&... args) {
    ensure_initialized();
    s_logger_->warn(fmt, std::forward<Args>(args)...);
}

template <typename... Args>
void Logger::error(const std::string& fmt, Args&&... args) {
    ensure_initialized();
    s_logger_->error(fmt, std::forward<Args>(args)...);
}

// --- Explicit Template Instantiations ---
// This is necessary because the template definitions are in a .cpp file.
// We need to tell the compiler which types we will use them with.
// For a generic logger, this can be complex. For simplicity, we'll
// instantiate for common types. A better approach might be to move the
// template bodies to an inline header file (`logger-inl.h`).
// Let's assume for now we only need basic string logging for simplicity.

template void Logger::info<const char*&>(const std::string&, const char*&);
template void Logger::warn<const char*&>(const std::string&, const char*&);
// ... and so on for other types you need.

} // namespace core
} // namespace ignlink