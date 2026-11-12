#pragma once

#include "dispatcher/model/ISystemElement.hpp"
#include "dispatcher/model/CommonTypes.hpp"
#include "dispatcher/visual/IVisualDescriptorProvider.hpp"
#include <string>

namespace dispatcher {



class Alarm : public ISystemElement, public IVisualDescriptorProvider {
public:
    Alarm(std::string id, std::string roomId, Severity severity);

    std::string id() const override;
    std::string kind() const override;
    std::string status() const override;
    std::string summary() const override;

    VisualDescriptor toVisualDescriptor() const override;

    const std::string& roomId() const;
    Severity severity() const;

private:
    std::string id_;
    std::string roomId_;
    Severity severity_{Severity::Low};
};

} // namespace dispatcher