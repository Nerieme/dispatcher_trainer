#pragma once

#include "dispatcher/model/ISystemElement.hpp"
#include "dispatcher/visual/IVisualDescriptorProvider.hpp"
#include <string>

namespace dispatcher {

enum class ElementStatus { Normal, Warning, Critical, Offline };

class Room : public ISystemElement, public IVisualDescriptorProvider {
public:
    Room(std::string id, std::string name, std::string building, ElementStatus status);

    std::string id() const override;
    std::string kind() const override;
    std::string status() const override;
    std::string summary() const override;

    VisualDescriptor toVisualDescriptor() const override;

    const std::string& building() const;
    ElementStatus elementStatus() const;
    void setStatus(ElementStatus status);

private:
    std::string id_;
    std::string name_;
    std::string building_;
    ElementStatus status_{ElementStatus::Normal};
};

std::string toString(ElementStatus status);

} // namespace dispatcher