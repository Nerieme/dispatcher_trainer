#include <cstdlib>
#include <iostream>
#include <string>

#include "dispatcher/stl/CommandResult.hpp"
#include "dispatcher/stl/Event.hpp"
#include "dispatcher/stl/Parameter.hpp"
#include "dispatcher/stl/SimulatorCore.hpp"

using namespace dispatcher;

namespace {

void check(bool condition, const char* message) {
    if (!condition) {
        std::cerr << "FAILED: " << message << "\n";
        std::exit(1);
    }
}

// ---------- 1. Диапазоны ----------

void test_value_inside_range_is_accepted() {
    SimulatorCore core;
    core.addParameter({"temperature", 28.5, 18.0, 70.0, "C"});
    core.rebuildParameterIndex();

    check(core.setParameterValue("temperature", 40.0),
          "value inside range should be accepted");

    auto p = core.findParameter("temperature");
    check(p.has_value(), "parameter should exist after update");
    check(p->value == 40.0, "value should be updated to 40.0");
}

void test_value_below_range_is_clamped_or_rejected() {
    SimulatorCore core;
    core.addParameter({"temperature", 28.5, 18.0, 70.0, "C"});
    core.rebuildParameterIndex();

    // setParameterValue принимает любое значение, но applyOperatorCommand
    // сообщает предупреждение при выходе за границы.
    CommandResult result = core.applyOperatorCommand("temperature", 5.0);
    check(std::holds_alternative<CommandWarning>(result),
          "value below min should produce CommandWarning");
}

void test_value_above_range_is_clamped_or_rejected() {
    SimulatorCore core;
    core.addParameter({"temperature", 28.5, 18.0, 70.0, "C"});
    core.rebuildParameterIndex();

    CommandResult result = core.applyOperatorCommand("temperature", 99.0);
    check(std::holds_alternative<CommandWarning>(result),
          "value above max should produce CommandWarning");
}

// ---------- 2. Изменение состояния ----------

void test_parameter_update_changes_state() {
    SimulatorCore core;
    core.addParameter({"smoke_level", 12.0, 0.0, 60.0, "%"});
    core.rebuildParameterIndex();

    check(core.setParameterValue("smoke_level", 45.0),
          "existing parameter should be updated");

    auto p = core.findParameter("smoke_level");
    check(p.has_value(), "updated parameter should be found");
    check(p->value == 45.0, "parameter value should be changed to 45.0");
}

void test_operator_command_ok() {
    SimulatorCore core;
    core.addParameter({"temperature", 28.5, 18.0, 70.0, "C"});
    core.rebuildParameterIndex();

    CommandResult result = core.applyOperatorCommand("temperature", 40.0);
    check(std::holds_alternative<CommandOk>(result),
          "command inside range should return CommandOk");
    check(commandResultToText(result) == "Значение изменено: temperature",
          "CommandOk text should match");
}

// ---------- 3. Поиск параметра ----------

void test_find_existing_parameter() {
    SimulatorCore core;
    core.addParameter({"pressure", 5.0, 0.0, 10.0, "MPa"});
    core.rebuildParameterIndex();

    auto found = core.findParameter("pressure");
    check(found.has_value(), "existing parameter should be found");
    check(found->value == 5.0, "found parameter should have expected value");

    auto index = core.findParameterIndex("pressure");
    check(index.has_value(), "index should be found");
    check(*index == 0, "first parameter should have index 0");
}

void test_find_missing_parameter() {
    SimulatorCore core;
    core.addParameter({"pressure", 5.0, 0.0, 10.0, "MPa"});
    core.rebuildParameterIndex();

    auto missing = core.findParameter("speed");
    check(!missing.has_value(), "missing parameter should return nullopt");

    auto missingIndex = core.findParameterIndex("speed");
    check(!missingIndex.has_value(), "missing index should return nullopt");
}

// ---------- 4. Добавление события в журнал ----------

void test_add_event_to_log() {
    SimulatorCore core;
    const std::size_t before = core.events().size();

    core.addEvent({1, EventType::Warning, "Датчик room A: задымление 45%", 5});

    const std::size_t after = core.events().size();
    check(after == before + 1, "event count should increase by one");
    check(core.events().back().message == "Датчик room A: задымление 45%",
          "last event text should match added event");
    check(core.events().back().type == EventType::Warning,
          "last event type should be Warning");
}

// ---------- Дополнительно: подсчёт, сортировка, статистика ----------

void test_count_alarm_events() {
    SimulatorCore core;
    core.addEvent({1, EventType::Info,    "start",   1});
    core.addEvent({2, EventType::Warning, "warn-1",  5});
    core.addEvent({3, EventType::Alarm,   "alarm-1", 10});
    core.addEvent({4, EventType::OperatorAction, "action", 2});

    check(core.countAlarmEvents() == 2,
          "should count two alarm events (Warning + Alarm)");
    check(core.filterAlarmEvents().size() == 2,
          "filtered list should contain two alarm events");
}

void test_sorted_log_is_copy() {
    SimulatorCore core;
    core.addEvent({1, EventType::Info,    "start",  1});
    core.addEvent({2, EventType::Warning, "warn-1", 5});
    core.addEvent({3, EventType::Alarm,   "alarm-1", 10});

    auto sorted = core.sortedLog();
    check(sorted.size() == 3, "sorted log should have same size");
    check(sorted[0].id == 3, "highest priority event should be first");

    // Исходный журнал не изменился
    check(core.events()[0].id == 1, "original log should stay unchanged");
    check(core.events()[1].id == 2, "original log order preserved");
    check(core.events()[2].id == 3, "original log order preserved");
}

void test_event_statistics() {
    SimulatorCore core;
    core.addEvent({1, EventType::Info,    "start",   1});
    core.addEvent({2, EventType::Warning, "warn-1",  5});
    core.addEvent({3, EventType::Alarm,   "alarm-1", 10});

    auto stats = core.eventStatistics();
    check(stats.at(EventType::Info) == 1, "one Info event");
    check(stats.at(EventType::Warning) == 1, "one Warning event");
    check(stats.at(EventType::Alarm) == 1, "one Alarm event");
}

} // namespace

int main() {
    // 1. Диапазоны
    test_value_inside_range_is_accepted();
    test_value_below_range_is_clamped_or_rejected();
    test_value_above_range_is_clamped_or_rejected();

    // 2. Изменение состояния
    test_parameter_update_changes_state();
    test_operator_command_ok();

    // 3. Поиск параметра
    test_find_existing_parameter();
    test_find_missing_parameter();

    // 4. Добавление события
    test_add_event_to_log();

    // Дополнительно
    test_count_alarm_events();
    test_sorted_log_is_copy();
    test_event_statistics();

    std::cout << "All tests passed\n";
    return 0;
}