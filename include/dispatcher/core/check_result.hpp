#pragma once

#include <string>

namespace dispatcher {

enum class Severity { Ok, Warning, Critical };

struct CheckResult {
    std::string parameterId;
    std::string message;
    Severity severity = Severity::Ok;
};

} // namespace dispatcher