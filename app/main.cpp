#include <iostream>
#include <string>

#include "dispatcher/stl/SimulatorCore.hpp"
#include "dispatcher/stl/Event.hpp"
#include "dispatcher/stl/Parameter.hpp"

using namespace dispatcher;

namespace {

void printParameters(const SimulatorCore& core) {
    std::cout << "Параметры:\n";
    for (const auto& line : core.parameterLines()) {
        std::cout << "  " << line << "\n";
    }
}

void printEvents(const std::vector<Event>& events) {
    for (const auto& e : events) {
        std::cout << "  [" << e.id << "] " << toString(e.type)
                  << " prio=" << e.priority
                  << " — " << e.message << "\n";
    }
}

void printHelp() {
    std::cout << "Команды:\n"
              << "  status                      — показать параметры\n"
              << "  find <name>                 — найти параметр по имени\n"
              << "  alarms                      — показать тревожные события\n"
              << "  count_alarms                — количество тревожных событий\n"
              << "  log                         — исходный журнал (без сортировки)\n"
              << "  log_sorted                  — отсортированная копия журнала\n"
              << "  stats                       — статистика по типам событий\n"
              << "  names                       — краткий список имён параметров\n"
              << "  lines                       — краткий список параметров name=value\n"
              << "  help                        — справка\n"
              << "  exit                        — выход\n";
}

SimulatorCore buildDemoCore() {
    SimulatorCore core;

    // Параметры диспетчера безопасности
    core.addParameter({"smoke_level",   12.0,  0.0,  100.0, "%"});
    core.addParameter({"temperature",   28.5, 18.0,   60.0, "C"});
    core.addParameter({"active_sensors", 2.0,  0.0,   10.0, "шт."});
    core.addParameter({"brigade_load",  40.0,  0.0,  100.0, "%"});

    // Журнал событий
    core.addEvent({1, EventType::Info,           "Тренажёр запущен",                    1});
    core.addEvent({2, EventType::OperatorAction, "Оператор запросил статус",            2});
    core.addEvent({3, EventType::Warning,        "Датчик room A: задымление 45%",       5});
    core.addEvent({4, EventType::Alarm,          "Датчик room A: задымление 78%",      10});
    core.addEvent({5, EventType::Warning,        "Камера room D: нет сигнала",          4});
    core.addEvent({6, EventType::Info,           "Brigade A: выехала в build 1",        2});

    return core;
}

} // namespace

int main() {
    SimulatorCore core = buildDemoCore();
    core.rebuildParameterIndex();

    std::cout << "Тренажёр диспетчера безопасности. Введите help для команд.\n";

    std::string line;
    while (true) {
        std::cout << "dispatcher> ";
        if (!std::getline(std::cin, line)) break;
        if (line.empty()) continue;

        if (line == "help") {
            printHelp();
        } else if (line == "status") {
            printParameters(core);
        } else if (line == "find" || line.rfind("find ", 0) == 0) {
            std::string name;
            if (line == "find") {
                std::cout << "Формат: find <name>\n";
                continue;
            }
            name = line.substr(5);
            auto p = core.findParameter(name);
            if (p) {
                std::cout << "Найден: " << p->name << " = " << p->value
                          << " " << p->unit << "\n";
            } else {
                std::cout << "Параметр не найден: " << name << "\n";
            }
        } else if (line == "alarms") {
            auto alarms = core.filterAlarmEvents();
            std::cout << "Тревожные события (" << alarms.size() << "):\n";
            printEvents(alarms);
        } else if (line == "count_alarms") {
            std::cout << "Количество тревожных событий: "
                      << core.countAlarmEvents() << "\n";
        } else if (line == "log") {
            const auto& events = core.events();
            std::cout << "Исходный журнал (" << events.size() << " событий):\n";
            printEvents(events);
        } else if (line == "log_sorted") {
            std::cout << "Отсортированная копия журнала:\n";
            printEvents(core.sortedLog());
        } else if (line == "stats") {
            auto stats = core.eventStatistics();
            std::size_t total = 0;
            for (const auto& [type, count] : stats) total += count;
            std::cout << "Статистика по типам событий (всего " << total << "):\n";
            for (const auto& [type, count] : stats) {
                std::cout << "  " << toString(type) << ": " << count << "\n";
            }
        } else if (line == "names") {
            std::cout << "Имена параметров:\n";
            for (const auto& n : core.parameterNames()) {
                std::cout << "  " << n << "\n";
            }
        } else if (line == "lines") {
            const auto& params = core.parameters();
            std::cout << "Параметры name=value (всего " << params.size() << "):\n";
            for (const auto& l : core.parameterLines()) {
                std::cout << "  " << l << "\n";
            }
        } else if (line == "exit") {
            std::cout << "Выход.\n";
            break;
        } else {
            std::cout << "Неизвестная команда. Введите help.\n";
        }
    }

    return 0;
}