#include <ignlink/core/status.h>

namespace ignlink {
namespace core {

std::string status_to_string(const Status& status) {
    std::string code_str;
    switch (status.code()) {
        case Status::Code::Ok:              code_str = "Ok"; break;
        case Status::Code::Error:           code_str = "Error"; break;
        case Status::Code::Timeout:         code_str = "Timeout"; break;
        case Status::Code::InvalidArgument: code_str = "InvalidArgument"; break;
        case Status::Code::NotFound:        code_str = "NotFound"; break;
        case Status::Code::AlreadyExists:   code_str = "AlreadyExists"; break;
        case Status::Code::Unavailable:     code_str = "Unavailable"; break;
        default:                            code_str = "Unknown"; break;
    }
    if (status.ok()) {
        return code_str;
    }
    return code_str + ": " + status.message();
}

} // namespace core
} // namespace ignlink