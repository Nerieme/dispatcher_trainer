#include "dispatcher/stl/SimulatorCore.hpp"
#include "dispatcher/stl/Event.hpp"
#include "dispatcher/stl/Parameter.hpp"

#include <cassert>
#include <iostream>

using namespace dispatcher;

int main() {
    SimulatorCore core;
    core.addParameter({"smoke_level",   12.0, 0.0, 100.0, "%"});
    core.addParameter({"temperature",   28.5, 18.0, 60.0, "C"});
    core.addParameter({"active_sensors", 2.0, 0.0, 10.0, "шт."});
    core.rebuildParameterIndex();

    // find_if / индекс
    assert(core.findParameter("temperature").has_value());
    assert(core.findParameter("temperature")->value == 28.5);
    assert(!core.findParameter("unknown").has_value());

    // events
    core.addEvent({1, EventType::Info,           "start",    1});
    core.addEvent({2, EventType::Warning,        "warn-1",   5});
    core.addEvent({3, EventType::Alarm,          "alarm-1", 10});
    core.addEvent({4, EventType::OperatorAction, "action",   2});

    assert(core.countAlarmEvents() == 2);
    assert(core.filterAlarmEvents().size() == 2);

    // sortedLog: копия, сортировка по priority desc, id asc
    auto sorted = core.sortedLog();
    assert(sorted.size() == 4);
    assert(sorted[0].id == 3);
    assert(sorted[1].id == 2);
    assert(sorted[2].id == 4);
    assert(sorted[3].id == 1);

    // исходный журнал не изменён
    assert(core.events().size() == 4);
    assert(core.events()[0].id == 1);
    assert(core.events()[1].id == 2);
    assert(core.events()[2].id == 3);
    assert(core.events()[3].id == 4);

    // stats
    auto stats = core.eventStatistics();
    assert(stats.at(EventType::Info) == 1);
    assert(stats.at(EventType::Warning) == 1);
    assert(stats.at(EventType::Alarm) == 1);
    assert(stats.at(EventType::OperatorAction) == 1);

    // transform
    auto names = core.parameterNames();
    assert(names.size() == 3);
    assert(names[0] == "smoke_level");

    auto lines = core.parameterLines();
    assert(lines.size() == 3);
    assert(lines[1] == "temperature=28.500000 C");

    std::cout << "All STL core tests passed\n";
    return 0;
}