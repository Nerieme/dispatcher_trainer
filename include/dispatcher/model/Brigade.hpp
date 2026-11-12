#pragma once

#include "dispatcher/model/ISystemElement.hpp"
#include "dispatcher/visual/IVisualDescriptorProvider.hpp"
#include <string>

namespace dispatcher {

class Brigade : public ISystemElement, public IVisualDescriptorProvider {
public:
    Brigade(std::string id, std::string name, std::string baseBuilding, bool busy = false);

    std::string id() const override;
    std::string kind() const override;
    std::string status() const override;
    std::string summary() const override;

    VisualDescriptor toVisualDescriptor() const override;

    const std::string& baseBuilding() const;
    bool isBusy() const;
    void setBusy(bool busy);

private:
    std::string id_;
    std::string name_;
    std::string baseBuilding_;
    bool busy_{false};
};

} // namespace dispatcher