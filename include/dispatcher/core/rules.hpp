#pragma once

#include "dispatcher/core/check_result.hpp"
#include "dispatcher/core/parameter.hpp"
#include "dispatcher/core/range.hpp"
#include <string>
#include <utility>

namespace dispatcher {

class ICheckRule {
public:
    virtual ~ICheckRule() = default;
    virtual CheckResult check() const = 0;
};

template <OrderedValue T>
class RangeRule final : public ICheckRule {
public:
    RangeRule(const Parameter<T>& parameter,
              Range<T> normalRange,
              std::string problemMessage)
        : parameter_(parameter),
          normalRange_(normalRange),
          problemMessage_(std::move(problemMessage)) {}

    CheckResult check() const override {
        if (normalRange_.contains(parameter_.value())) {
            return {parameter_.id(), "OK", Severity::Ok};
        }
        return {parameter_.id(), problemMessage_, Severity::Warning};
    }

private:
    const Parameter<T>& parameter_;
    Range<T> normalRange_;
    std::string problemMessage_;
};

} // namespace dispatcher