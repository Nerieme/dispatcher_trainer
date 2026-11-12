#include "dispatcher/model/Brigade.hpp"
#include <stdexcept>

namespace dispatcher {

Brigade::Brigade(std::string id, std::string name, std::string baseBuilding, bool busy)
    : id_(std::move(id)), name_(std::move(name)),
      baseBuilding_(std::move(baseBuilding)), busy_(busy) {
    if (id_.empty())           throw std::invalid_argument("Brigade: id must not be empty");
    if (name_.empty())         throw std::invalid_argument("Brigade: name must not be empty");
    if (baseBuilding_.empty()) throw std::invalid_argument("Brigade: baseBuilding must not be empty");
}

std::string Brigade::id() const { return id_; }
std::string Brigade::kind() const { return "Brigade"; }

std::string Brigade::status() const {
    return busy_ ? "Busy" : "Available";
}

std::string Brigade::summary() const {
    return "[Brigade " + id_ + "] " + name_ + " (base=" + baseBuilding_ +
           ") status=" + status();
}

VisualDescriptor Brigade::toVisualDescriptor() const {
    VisualDescriptor d;
    d.id         = id_;
    d.visualKind = "brigade";
    d.label      = "Бригада " + name_ + " (" + baseBuilding_ + ")";
    d.color      = busy_ ? "yellow" : "green";
    d.priority   = busy_ ? 40 : 20;
    d.anchorId   = id_;
    return d;
}

const std::string& Brigade::baseBuilding() const { return baseBuilding_; }
bool Brigade::isBusy() const { return busy_; }
void Brigade::setBusy(bool busy) { busy_ = busy; }

} // namespace dispatcher