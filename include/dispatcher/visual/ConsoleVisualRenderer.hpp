#pragma once

#include "dispatcher/visual/VisualDescriptor.hpp"
#include <vector>

namespace dispatcher {

class ConsoleVisualRenderer {
public:
    void render(const std::vector<VisualDescriptor>& descriptors) const;
};

} // namespace dispatcher