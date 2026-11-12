#include "dispatcher/stl/SimulatorCore.hpp"
#include "dispatcher/stl/CommandResult.hpp"
#include "dispatcher/stl/Event.hpp"

#include <cassert>
#include <filesystem>
#include <iostream>

using namespace dispatcher;

int main() {
    SimulatorCore core;
    core.addParameter({"smoke_level", 12.0, 0.0, 60.0, "%"});
    core.addParameter({"temperature", 28.5, 18.0, 70.0, "C"});
    core.rebuildParameterIndex();

    // --- optional ---
    assert(core.findParameterIndex("temperature").has_value());
    assert(!core.findParameterIndex("unknown_parameter").has_value());

    // optional + setParameterValue
    assert(core.setParameterValue("temperature", 32.0));
    assert(core.findParameter("temperature")->value == 32.0);
    assert(!core.setParameterValue("unknown_parameter", 1.0));

    // --- variant ---
    CommandResult ok = core.applyOperatorCommand("temperature", 40.0);
    assert(std::holds_alternative<CommandOk>(ok));
    assert(commandResultToText(ok) == "Значение изменено: temperature");

    CommandResult warn = core.applyOperatorCommand("temperature", 99.0);
    assert(std::holds_alternative<CommandWarning>(warn));

    CommandResult err = core.applyOperatorCommand("unknown", 1.0);
    assert(std::holds_alternative<CommandError>(err));

    // --- filesystem ---
    std::filesystem::path logPath = std::filesystem::path("logs") / "security_session.txt";
    core.saveTextLog(logPath);
    assert(std::filesystem::exists(logPath));

    // --- события сохраняются в тексте ---
    core.addEvent({1, EventType::Alarm, "test alarm", 10});
    core.saveTextLog(logPath);
    assert(std::filesystem::exists(logPath));

    std::cout << "All modern C++ tests passed\n";
    return 0;
}