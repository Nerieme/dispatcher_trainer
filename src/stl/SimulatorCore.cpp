#include "dispatcher/stl/SimulatorCore.hpp"

#include <algorithm>

namespace dispatcher {

void SimulatorCore::addParameter(Parameter parameter) {
    parameterIndex_[parameter.name] = parameters_.size();
    parameters_.push_back(std::move(parameter));
}

std::optional<Parameter> SimulatorCore::findParameter(const std::string& name) const {
    // Сначала — быстрый поиск по индексу
    if (auto it = parameterIndex_.find(name); it != parameterIndex_.end()) {
        return parameters_.at(it->second);
    }
    // Резерв — find_if по vector
    auto it = std::find_if(parameters_.begin(), parameters_.end(),
                           [&name](const Parameter& p) { return p.name == name; });
    if (it != parameters_.end()) return *it;
    return std::nullopt;
}

void SimulatorCore::rebuildParameterIndex() {
    parameterIndex_.clear();
    for (std::size_t i = 0; i < parameters_.size(); ++i) {
        parameterIndex_[parameters_[i].name] = i;
    }
}

void SimulatorCore::addEvent(Event event) {
    events_.push_back(std::move(event));
}

std::size_t SimulatorCore::countAlarmEvents() const {
    return static_cast<std::size_t>(
        std::count_if(events_.begin(), events_.end(),
                      [](const Event& e) { return isAlarmEvent(e); }));
}

std::vector<Event> SimulatorCore::filterAlarmEvents() const {
    std::vector<Event> result;
    result.reserve(events_.size());
    std::copy_if(events_.begin(), events_.end(),
                 std::back_inserter(result),
                 [](const Event& e) { return isAlarmEvent(e); });
    return result;
}

std::vector<Event> SimulatorCore::sortedLog() const {
    std::vector<Event> copy = events_;
    std::sort(copy.begin(), copy.end(),
              [](const Event& left, const Event& right) {
                  if (left.priority != right.priority) {
                      return left.priority > right.priority;   // выше приоритет — раньше
                  }
                  return left.id < right.id;                   // при равенстве — по id
              });
    return copy;
}

std::map<EventType, std::size_t> SimulatorCore::eventStatistics() const {
    std::map<EventType, std::size_t> statistics;
    for (const auto& event : events_) {
        ++statistics[event.type];
    }
    return statistics;
}

std::vector<std::string> SimulatorCore::parameterNames() const {
    std::vector<std::string> names;
    names.reserve(parameters_.size());
    std::transform(parameters_.begin(), parameters_.end(),
                   std::back_inserter(names),
                   [](const Parameter& p) { return p.name; });
    return names;
}

std::vector<std::string> SimulatorCore::parameterLines() const {
    std::vector<std::string> lines;
    lines.reserve(parameters_.size());
    std::transform(parameters_.begin(), parameters_.end(),
                   std::back_inserter(lines),
                   [](const Parameter& p) {
                       return p.name + "=" + std::to_string(p.value) + " " + p.unit;
                   });
    return lines;
}

const std::vector<Parameter>& SimulatorCore::parameters() const { return parameters_; }
const std::vector<Event>& SimulatorCore::events() const { return events_; }

} // namespace dispatcher