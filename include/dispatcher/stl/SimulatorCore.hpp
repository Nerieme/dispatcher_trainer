#pragma once

#include "dispatcher/stl/Event.hpp"
#include "dispatcher/stl/Parameter.hpp"

#include <cstddef>
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
    void rebuildParameterIndex();

    // --- Журнал событий ---
    void addEvent(Event event);
    std::size_t countAlarmEvents() const;
    std::vector<Event> filterAlarmEvents() const;
    std::vector<Event> sortedLog() const;            // копия, сортировка по priority desc, id asc
    std::map<EventType, std::size_t> eventStatistics() const;

    // --- Преобразование ---
    std::vector<std::string> parameterNames() const;
    std::vector<std::string> parameterLines() const; // "name=value unit"

    // --- Доступ к данным (для вывода) ---
    const std::vector<Parameter>& parameters() const;
    const std::vector<Event>& events() const;

private:
    std::vector<Parameter> parameters_;
    std::vector<Event> events_;
    std::unordered_map<std::string, std::size_t> parameterIndex_;
};

} // namespace dispatcher