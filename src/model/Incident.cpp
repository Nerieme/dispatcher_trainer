#include "dispatcher/model/Incident.hpp"
#include <stdexcept>

namespace dispatcher {

Incident::Incident(std::string id, std::string roomId, std::string building,
                   Severity severity, bool closed)
    : id_(std::move(id)), roomId_(std::move(roomId)),
      building_(std::move(building)), severity_(severity), closed_(closed) {
    if (id_.empty())       throw std::invalid_argument("Incident: id must not be empty");
    if (roomId_.empty())   throw std::invalid_argument("Incident: roomId must not be empty");
    if (building_.empty()) throw std::invalid_argument("Incident: building must not be empty");
}

std::string Incident::id() const { return id_; }
std::string Incident::kind() const { return "Incident"; }

std::string Incident::status() const {
    if (closed_) return "Closed";
    switch (severity_) {
        case Severity::Critical: return "Critical";
        case Severity::High:     return "Warning";
        case Severity::Medium:   return "Warning";
        case Severity::Low:      return "Normal";
    }
    return "Unknown";
}

std::string Incident::summary() const {
    return "[Incident " + id_ + "] room=" + roomId_ +
           " building=" + building_ +
           " severity=" + toString(severity_) +
           (closed_ ? " (closed)" : "");
}

VisualDescriptor Incident::toVisualDescriptor() const {
    VisualDescriptor d;
    d.id         = id_;
    d.visualKind = "incident";
    d.label      = "Инцидент " + id_ + ": комната " + roomId_;
    d.color      = closed_ ? "gray"
                 : (severity_ == Severity::Critical) ? "red"
                 : (severity_ == Severity::High)     ? "orange"
                 : (severity_ == Severity::Medium)   ? "yellow"
                                                     : "green";
    d.priority   = closed_ ? 0
                 : (severity_ == Severity::Critical) ? 100
                 : (severity_ == Severity::High)     ? 85
                 : (severity_ == Severity::Medium)   ? 55
                                                     : 15;
    d.anchorId   = roomId_;
    return d;
}

const std::string& Incident::roomId() const { return roomId_; }
const std::string& Incident::building() const { return building_; }
Severity Incident::severity() const { return severity_; }
bool Incident::isClosed() const { return closed_; }
void Incident::close() { closed_ = true; }

} // namespace dispatcher