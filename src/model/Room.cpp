#include "dispatcher/model/Room.hpp"
#include <stdexcept>

namespace dispatcher {

std::string toString(ElementStatus status) {
    switch (status) {
        case ElementStatus::Normal:   return "Normal";
        case ElementStatus::Warning:  return "Warning";
        case ElementStatus::Critical: return "Critical";
        case ElementStatus::Offline:  return "Offline";
    }
    return "Unknown";
}

Room::Room(std::string id, std::string name, std::string building, ElementStatus status)
    : id_(std::move(id)), name_(std::move(name)),
      building_(std::move(building)), status_(status) {
    if (id_.empty())       throw std::invalid_argument("Room: id must not be empty");
    if (name_.empty())     throw std::invalid_argument("Room: name must not be empty");
    if (building_.empty()) throw std::invalid_argument("Room: building must not be empty");
}

std::string Room::id() const { return id_; }
std::string Room::kind() const { return "Room"; }

std::string Room::status() const {
    return toString(status_);
}

std::string Room::summary() const {
    return "[Room " + id_ + "] " + name_ + " (" + building_ +
           ") status=" + toString(status_);
}

VisualDescriptor Room::toVisualDescriptor() const {
    VisualDescriptor d;
    d.id         = id_;
    d.visualKind = "room";
    d.label      = "Комната " + name_ + " (" + building_ + ")";
    d.color      = (status_ == ElementStatus::Critical) ? "red"
                 : (status_ == ElementStatus::Warning)  ? "yellow"
                 : (status_ == ElementStatus::Offline)  ? "gray"
                                                        : "green";
    d.priority   = (status_ == ElementStatus::Critical) ? 90
                 : (status_ == ElementStatus::Warning)  ? 50
                                                        : 10;
    d.anchorId   = id_;
    return d;
}

const std::string& Room::building() const { return building_; }
ElementStatus Room::elementStatus() const { return status_; }
void Room::setStatus(ElementStatus status) { status_ = status; }

} // namespace dispatcher