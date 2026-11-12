#pragma once

#include "dispatcher/model/ISystemElement.hpp"
#include "dispatcher/visual/VisualDescriptor.hpp"
#include <memory>
#include <vector>

namespace dispatcher {

class DispatcherModel {
public:
    DispatcherModel() = default;

    void addElement(std::unique_ptr<ISystemElement> element);

    void printSummary() const;

    std::vector<VisualDescriptor> collectVisualDescriptors() const;

    std::size_t size() const;

private:
    std::vector<std::unique_ptr<ISystemElement>> elements_;
};

} // namespace dispatcher