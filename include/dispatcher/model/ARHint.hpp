#pragma once

#include "dispatcher/model/ISystemElement.hpp"
#include "dispatcher/visual/IVisualDescriptorProvider.hpp"
#include <string>

namespace dispatcher {

class ARHint : public ISystemElement, public IVisualDescriptorProvider {
public:
    ARHint(std::string id, std::string text, std::string anchorId, int priority = 50);

    std::string id() const override;
    std::string kind() const override;
    std::string status() const override;
    std::string summary() const override;

    VisualDescriptor toVisualDescriptor() const override;

    const std::string& anchorId() const;
    int priority() const;

private:
    std::string id_;
    std::string text_;
    std::string anchorId_;
    int priority_{50};
};

} // namespace dispatcher