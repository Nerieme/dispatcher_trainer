#pragma once

#include <string>

namespace dispatcher {

class ISystemElement {
public:
    virtual ~ISystemElement() = default;

    virtual std::string id() const = 0;
    virtual std::string kind() const = 0;
    virtual std::string status() const = 0;
    virtual std::string summary() const = 0;
};

} // namespace dispatcher