#pragma once

#include "dispatcher/stl/CommandResult.hpp"
#include "dispatcher/stl/Event.hpp"
#include "dispatcher/stl/Parameter.hpp"

#include <cstddef>
#include <filesystem>
#include <map>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

namespace dispatcher {

class SimulatorCore {
public:
    // --- Параметры ---
    void addParameter(Parameter parameter);
    std::optional<Parameter> findParameter(const std::string& name) const;
    std::optional<std::size_t> findParameterIndex(const std::string& name) const;
    bool setParameterValue(const std::string& name, double value);
    void rebuildParameterIndex();

    // --- Команды ---
    CommandResult applyOperatorCommand(const std::string& parameterName, double newValue);

    // --- Журнал событий ---
    void addEvent(Event event);
    std::size_t countAlarmEvents() const;
    std::vector<Event> filterAlarmEvents() const;
    std::vector<Event> sortedLog() const;
    std::map<EventType, std::size_t> eventStatistics() const;

    // --- Сохранение журнала ---
    void saveTextLog(const std::filesystem::path& path) const;

    // --- Преобразование ---
    std::vector<std::string> parameterNames() const;
    std::vector<std::string> parameterLines() const;

    // --- Доступ к данным ---
    const std::vector<Parameter>& parameters() const;
    const std::vector<Event>& events() const;

private:
    std::vector<Parameter> parameters_;
    std::vector<Event> events_;
    std::unordered_map<std::string, std::size_t> parameterIndex_;
};

} // namespace dispatcher