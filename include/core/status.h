#pragma once

#include <string>

namespace ignlink {
namespace core {

/**
 * @class Status
 * @brief Represents the result of an operation, including success or an error code.
 *
 * This is a lightweight object used as a return type throughout the Ignition Link
 * library to provide clear, consistent error handling without relying heavily
 * on exceptions for non-fatal errors.
 */
class Status {
public:
    enum class Code {
        Ok,
        Error,
        Timeout,
        InvalidArgument,
        NotFound,
        AlreadyExists,
        Unavailable
    };

    // Default constructor for a success status
    Status() : code_(Code::Ok) {}

    // Constructor for a specific status code and message
    Status(Code code, const std::string& message) : code_(code), message_(message) {}

    /**
     * @brief Checks if the status represents success.
     * @return True if the status code is Ok, false otherwise.
     */
    bool ok() const { return code_ == Code::Ok; }

    /**
     * @brief Gets the status code.
     */
    Code code() const { return code_; }

    /**
     * @brief Gets the error message associated with the status.
     */
    const std::string& message() const { return message_; }

    /**
     * @brief A static helper to create a success status.
     */
    static Status OK() { return Status(); }

private:
    Code code_;
    std::string message_;
};

// Helper function to convert a status code to a string for logging.
std::string status_to_string(const Status& status);

} // namespace core
} // namespace ignlink