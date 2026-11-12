#include "dispatcher/core/parameter.hpp"
#include "dispatcher/core/range.hpp"
#include "dispatcher/core/rules.hpp"

#include <cassert>
#include <iostream>

using namespace dispatcher;

int main() {
    // Range<int>
    Range<int> intRange{1, 10};
    assert(intRange.contains(1));
    assert(intRange.contains(5));
    assert(intRange.contains(10));
    assert(!intRange.contains(0));
    assert(!intRange.contains(11));

    // Range<double>
    Range<double> doubleRange{6.0, 8.0};
    assert(doubleRange.contains(7.5));
    assert(!doubleRange.contains(9.2));

    // RangeRule<double>
    Parameter<double> p("P", "Pressure", 7.5, "MPa");
    RangeRule<double> rule(p, Range<double>{6.0, 8.0}, "bad");
    assert(rule.check().severity == Severity::Ok);
    p.setValue(9.2);
    assert(rule.check().severity == Severity::Warning);

    // RangeRule<int>
    Parameter<int> level("L", "Level", 50, "%");
    RangeRule<int> intRule(level, Range<int>{20, 90}, "level out of range");
    assert(intRule.check().severity == Severity::Ok);
    level.setValue(95);
    assert(intRule.check().severity == Severity::Warning);

    // Range с некорректными границами должен бросить исключение
    bool thrown = false;
    try {
        Range<int> badRange{10, 1};
    } catch (const std::invalid_argument&) {
        thrown = true;
    }
    assert(thrown);

    std::cout << "All template rules tests passed\n";
    return 0;
}