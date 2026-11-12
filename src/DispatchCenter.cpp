#include "dispatcher/DispatchCenter.hpp"
#include <iostream>

namespace dispatcher {

DispatchCenter::DispatchCenter(EventLog& log) : log_(log) {}

void DispatchCenter::addUnit(DispatchUnit unit) {
    log_.write("add unit id=" + std::to_string(unit.id()) +
               " name=" + unit.name() +
               " zone=" + unit.zone());
    units_.push_back(std::move(unit));
}

void DispatchCenter::addIncident(Incident incident) {
    log_.write("add incident id=" + std::to_string(incident.id()) +
               " title=" + incident.title() +
               " zone=" + incident.zone() +
               " severity=" + std::string(toString(incident.severity())) +
               " marker=" + std::string(toString(incident.marker())));
    incidents_.push_back(std::move(incident));
}

DispatchUnit* DispatchCenter::findUnit(int id) {
    for (auto& u : units_) if (u.id() == id) return &u;
    return nullptr;
}

Incident* DispatchCenter::findIncident(int id) {
    for (auto& i : incidents_) if (i.id() == id) return &i;
    return nullptr;
}

const DispatchUnit* DispatchCenter::findUnit(int id) const {
    for (const auto& u : units_) if (u.id() == id) return &u;
    return nullptr;
}

const Incident* DispatchCenter::findIncident(int id) const {
    for (const auto& i : incidents_) if (i.id() == id) return &i;
    return nullptr;
}

bool DispatchCenter::assign(int unitId, int incidentId) {
    auto* unit = findUnit(unitId);
    if (!unit) {
        log_.write("assign failed: unit " + std::to_string(unitId) + " not found");
        return false;
    }

    auto* incident = findIncident(incidentId);
    if (!incident) {
        log_.write("assign failed: incident " + std::to_string(incidentId) + " not found");
        return false;
    }

    if (unit->status() != UnitStatus::Available) {
        log_.write("assign failed: unit " + std::to_string(unitId) + " not available");
        return false;
    }

    if (incident->status() != IncidentStatus::Open) {
        log_.write("assign failed: incident " + std::to_string(incidentId) + " not open");
        return false;
    }

    if (!unit->assignToIncident(incidentId)) {
        log_.write("assign failed: unit->assignToIncident returned false");
        return false;
    }
    if (!incident->assignUnit(unitId)) {
        unit->release();
        log_.write("assign failed: incident->assignUnit returned false");
        return false;
    }

    log_.write("assigned unit " + std::to_string(unitId) +
               " to incident " + std::to_string(incidentId));
    return true;
}

bool DispatchCenter::closeIncident(int incidentId) {
    auto* incident = findIncident(incidentId);
    if (!incident) {
        log_.write("close failed: incident " + std::to_string(incidentId) + " not found");
        return false;
    }
    if (incident->status() == IncidentStatus::Closed) {
        log_.write("close failed: incident " + std::to_string(incidentId) + " already closed");
        return false;
    }

    if (auto assigned = incident->assignedUnitId(); assigned.has_value()) {
        if (auto* unit = findUnit(*assigned)) {
            unit->release();
        }
    }

    incident->close();
    log_.write("closed incident " + std::to_string(incidentId));
    return true;
}

void DispatchCenter::printUnits() const {
    std::cout << "Units:\n";
    for (const auto& u : units_) {
        std::cout << "  #" << u.id()
                  << " " << u.name()
                  << " zone=" << u.zone()
                  << " status=" << toString(u.status());
        if (auto assigned = u.assignedIncidentId(); assigned.has_value()) {
            std::cout << " -> incident#" << *assigned;
        }
        std::cout << '\n';
    }
}

void DispatchCenter::printIncidents() const {
    std::cout << "Incidents:\n";
    for (const auto& i : incidents_) {
        std::cout << "  #" << i.id()
                  << " " << i.title()
                  << " zone=" << i.zone()
                  << " severity=" << toString(i.severity())
                  << " status=" << toString(i.status())
                  << " marker=" << toString(i.marker());
        if (auto assigned = i.assignedUnitId(); assigned.has_value()) {
            std::cout << " -> unit#" << *assigned;
        }
        std::cout << '\n';
    }
}

void DispatchCenter::printState() const {
    std::cout << "===== Dispatch Center State =====\n";
    printUnits();
    printIncidents();
    std::cout << "=================================\n";
}

}