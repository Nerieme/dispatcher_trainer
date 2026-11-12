#pragma once

#include <string>

namespace dispatcher {

enum class Severity { Low, Medium, High, Critical };

std::string toString(Severity severity);

} // namespace dispatcher