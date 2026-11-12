#pragma once

#include <cstddef>
#include <string>

namespace dispatcher {

enum class EventType {
    Info,
    OperatorAction,
    Warning,
    Alarm
};

struct Event {
    std::size_t id{0};
    EventType type{EventType::Info};
    std::string message;
    int priority{0};
};

bool isAlarmEvent(const Event& event);

std::string toString(EventType type);

} // namespace dispatcher