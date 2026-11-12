#include "dispatcher/model/DispatcherModel.hpp"
#include "dispatcher/visual/IVisualDescriptorProvider.hpp"
#include <iostream>

namespace dispatcher {

void DispatcherModel::addElement(std::unique_ptr<ISystemElement> element) {
    if (!element) return;
    elements_.push_back(std::move(element));
}

void DispatcherModel::printSummary() const {
    std::cout << "===== Dispatcher Model Summary =====\n";
    for (const auto& e : elements_) {
        std::cout << "  kind=" << e->kind()
                  << " id="   << e->id()
                  << " status=" << e->status() << "\n"
                  << "    " << e->summary() << "\n";
    }
    std::cout << "====================================\n";
}

std::vector<VisualDescriptor>
DispatcherModel::collectVisualDescriptors() const {
    std::vector<VisualDescriptor> result;
    result.reserve(elements_.size());

    for (const auto& e : elements_) {
        if (auto provider = dynamic_cast<const IVisualDescriptorProvider*>(e.get())) {
            result.push_back(provider->toVisualDescriptor());
        }
    }
    return result;
}

std::size_t DispatcherModel::size() const { return elements_.size(); }

} // namespace dispatcher