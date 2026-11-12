#pragma once

#include "dispatcher/core/check_result.hpp"
#include <string>

namespace dispatcher {

struct VisualDescriptor {
    std::string objectId;
    std::string visualKind; // "gauge", "badge", "route_hint", "ar_marker"
    std::string label;
    std::string color;      // "green", "yellow", "red", "blue"
    int priority = 0;
};

inline std::string colorBySeverity(Severity severity) {
    switch (severity) {
        case Severity::Ok:       return "green";
        case Severity::Warning:  return "yellow";
        case Severity::Critical: return "red";
    }
    return "blue";
}

} // namespace dispatcher