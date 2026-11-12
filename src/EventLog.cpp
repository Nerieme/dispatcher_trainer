#include "dispatcher/EventLog.hpp"

// #include <chrono>
// #include <iomanip>
#include <stdexcept>

namespace dispatcher {

EventLog::EventLog(const std::filesystem::path& path)
 : out_(path, std::ios::app)
{
    if (!out_) {
        throw std::runtime_error("Cannot open event log file: " + path.string());
    }
    write("log opened");
}

EventLog::~EventLog() {
    if (out_) {
        out_ << "log closed\n";
    }
}
void EventLog::write(std::string_view message) {
    out_ << message << '\n';
}

}