#pragma once

#include <string>

namespace dispatcher {

struct VisualDescriptor {
    std::string id;
    std::string visualKind;   // "room", "brigade", "alarm", "incident", "ar-hint"
    std::string label;
    std::string color;        // "green", "yellow", "red", "blue", "cyan", "gray"
    int priority{0};
    std::string anchorId;     // к какому объекту привязана AR-подсказка
};

} // namespace dispatcher