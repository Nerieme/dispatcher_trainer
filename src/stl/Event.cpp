#include "dispatcher/stl/Event.hpp"

namespace dispatcher {

bool isAlarmEvent(const Event& event) {
    return event.type == EventType::Warning || event.type == EventType::Alarm;
}

std::string toString(EventType type) {
    switch (type) {
        case EventType::Info:           return "Info";
        case EventType::OperatorAction: return "OperatorAction";
        case EventType::Warning:        return "Warning";
        case EventType::Alarm:          return "Alarm";
    }
    return "Unknown";
}

} // namespace dispatcher