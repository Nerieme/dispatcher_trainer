#pragma once

#include "dispatcher/model/ISystemElement.hpp"
#include "dispatcher/model/CommonTypes.hpp"
#include "dispatcher/visual/IVisualDescriptorProvider.hpp"
#include <string>

namespace dispatcher {

class Incident : public ISystemElement, public IVisualDescriptorProvider {
public:
    Incident(std::string id, std::string roomId, std::string building,
             Severity severity, bool closed = false);

    std::string id() const override;
    std::string kind() const override;
    std::string status() const override;
    std::string summary() const override;

    VisualDescriptor toVisualDescriptor() const override;

    const std::string& roomId() const;
    const std::string& building() const;
    Severity severity() const;
    bool isClosed() const;
    void close();

private:
    std::string id_;
    std::string roomId_;
    std::string building_;
    Severity severity_{Severity::Low};
    bool closed_{false};
};

} // namespace dispatcher