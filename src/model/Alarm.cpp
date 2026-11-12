#include "dispatcher/model/Alarm.hpp"
#include <stdexcept>

namespace dispatcher {

Alarm::Alarm(std::string id, std::string roomId, Severity severity)
    : id_(std::move(id)), roomId_(std::move(roomId)), severity_(severity) {
    if (id_.empty())     throw std::invalid_argument("Alarm: id must not be empty");
    if (roomId_.empty()) throw std::invalid_argument("Alarm: roomId must not be empty");
}

std::string Alarm::id() const { return id_; }
std::string Alarm::kind() const { return "Alarm"; }

std::string Alarm::status() const {
    switch (severity_) {
        case Severity::Critical: return "Critical";
        case Severity::High:     return "Warning";
        case Severity::Medium:   return "Warning";
        case Severity::Low:      return "Normal";
    }
    return "Unknown";
}

std::string Alarm::summary() const {
    return "[Alarm " + id_ + "] room=" + roomId_ +
           " severity=" + toString(severity_);
}

VisualDescriptor Alarm::toVisualDescriptor() const {
    VisualDescriptor d;
    d.id         = id_;
    d.visualKind = "alarm";
    d.label      = "Тревога " + id_ + " в комнате " + roomId_;
    d.color      = (severity_ == Severity::Critical) ? "red"
                 : (severity_ == Severity::High)     ? "orange"
                 : (severity_ == Severity::Medium)   ? "yellow"
                                                     : "green";
    d.priority   = (severity_ == Severity::Critical) ? 100
                 : (severity_ == Severity::High)     ? 85
                 : (severity_ == Severity::Medium)   ? 55
                                                     : 15;
    d.anchorId   = roomId_;
    return d;
}

const std::string& Alarm::roomId() const { return roomId_; }
Severity Alarm::severity() const { return severity_; }

} // namespace dispatcher