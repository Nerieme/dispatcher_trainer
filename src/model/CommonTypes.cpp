#include "dispatcher/model/CommonTypes.hpp"

namespace dispatcher {

std::string toString(Severity severity) {
    switch (severity) {
        case Severity::Low:      return "Low";
        case Severity::Medium:   return "Medium";
        case Severity::High:     return "High";
        case Severity::Critical: return "Critical";
    }
    return "Unknown";
}

} // namespace dispatcher