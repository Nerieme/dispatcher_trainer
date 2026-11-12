#pragma once

#include <string>

namespace dispatcher {

struct Parameter {
    std::string name;
    double value{0.0};
    double minValue{0.0};
    double maxValue{100.0};
    std::string unit;
};

} // namespace dispatcher