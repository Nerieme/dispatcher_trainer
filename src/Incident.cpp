#include "dispatcher/Incident.hpp"
#include <stdexcept>

namespace dispatcher {

Incident::Incident(int id, std::string title, std::string zone,
                   Severity severity, VisualMarker marker)
    : id_(id),
      title_(std::move(title)),
      zone_(std::move(zone)),
      severity_(severity),
      marker_(marker) {
    if (id_ <= 0) {
        throw std::invalid_argument("Incident: id must be positive");
    }
    if (title_.empty()) {
        throw std::invalid_argument("Incident: title must not be empty");
    }
    if (zone_.empty()) {
        throw std::invalid_argument("Incident: zone must not be empty");
    }
}

int Incident::id() const { return id_; }
const std::string& Incident::title() const { return title_; }
const std::string& Incident::zone() const { return zone_; }
Severity Incident::severity() const { return severity_; }
IncidentStatus Incident::status() const { return status_; }
VisualMarker Incident::marker() const { return marker_; }
std::optional<int> Incident::assignedUnitId() const { return assignedUnitId_; }

bool Incident::assignUnit(int unitId) {
    if (unitId <= 0) return false;
    if (status_ != IncidentStatus::Open) return false;
    if (assignedUnitId_.has_value()) return false;

    status_ = IncidentStatus::Assigned;
    assignedUnitId_ = unitId;
    return true;
}

void Incident::close() {
    status_ = IncidentStatus::Closed;
    assignedUnitId_.reset();
}

}