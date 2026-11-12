#include <iostream>
#include <memory>
#include <vector>

#include "dispatcher/core/check_result.hpp"
#include "dispatcher/core/parameter.hpp"
#include "dispatcher/core/range.hpp"
#include "dispatcher/core/rules.hpp"
#include "dispatcher/presentation/visual_descriptor.hpp"

using namespace dispatcher;

int main() {
    Parameter<double> smokeLevel("SMOKE-A", "Уровень задымления room A", 12.0, "%");
    Parameter<double> temperature("TEMP-D", "Температура room D", 28.5, "C");
    Parameter<int> activeSensors("SENSORS-B", "Активные датчики room B", 2, "шт.");
    Parameter<int> brigadeLoad("LOAD-1", "Загрузка Brigade A", 40, "%");

    std::vector<std::unique_ptr<ICheckRule>> rules;
    rules.push_back(std::make_unique<RangeRule<double>>(
        smokeLevel, Range<double>{0.0, 20.0}, "Задымление выше нормы"));
    rules.push_back(std::make_unique<RangeRule<double>>(
        temperature, Range<double>{18.0, 30.0}, "Температура вне нормы"));
    rules.push_back(std::make_unique<RangeRule<int>>(
        activeSensors, Range<int>{0, 2}, "Слишком много активных датчиков"));
    rules.push_back(std::make_unique<RangeRule<int>>(
        brigadeLoad, Range<int>{0, 70}, "Бригада перегружена"));

    std::vector<VisualDescriptor> descriptors;

    for (const auto& rule : rules) {
        const CheckResult result = rule->check();
        std::cout << result.parameterId << ": " << result.message << "\n";

        VisualDescriptor d;
        d.objectId   = result.parameterId;
        d.visualKind = "badge";
        d.label      = result.message;
        d.color      = colorBySeverity(result.severity);
        d.priority   = (result.severity == Severity::Critical) ? 100
                     : (result.severity == Severity::Warning)  ? 50
                                                               : 10;
        descriptors.push_back(std::move(d));
    }

    std::cout << "\n===== Visual descriptors (mock) =====\n";
    for (const auto& d : descriptors) {
        std::cout << d.objectId << " | " << d.visualKind
                  << " | " << d.color
                  << " | priority=" << d.priority
                  << " | " << d.label << "\n";
    }

    return 0;
}