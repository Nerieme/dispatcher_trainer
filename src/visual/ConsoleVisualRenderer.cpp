#include "dispatcher/visual/ConsoleVisualRenderer.hpp"
#include <algorithm>
#include <iomanip>
#include <iostream>

namespace dispatcher {

void ConsoleVisualRenderer::render(const std::vector<VisualDescriptor>& descriptors) const {
    std::cout << "\n===== Visual Layer (Console Mock) =====\n";
    if (descriptors.empty()) {
        std::cout << "  (no visual descriptors)\n";
        std::cout << "=======================================\n";
        return;
    }

    auto sorted = descriptors;
    std::sort(sorted.begin(), sorted.end(),
              [](const VisualDescriptor& a, const VisualDescriptor& b) {
                  return a.priority > b.priority;
              });

    std::cout << std::left
              << std::setw(10) << "ID"
              << std::setw(12) << "KIND"
              << std::setw(10) << "COLOR"
              << std::setw(9)  << "PRIORITY"
              << std::setw(12) << "ANCHOR"
              << "LABEL\n";

    for (const auto& d : sorted) {
        std::cout << std::left
                  << std::setw(10) << d.id
                  << std::setw(12) << d.visualKind
                  << std::setw(10) << d.color
                  << std::setw(9)  << d.priority
                  << std::setw(12) << d.anchorId
                  << d.label << '\n';
    }
    std::cout << "=======================================\n";
}

} // namespace dispatcher