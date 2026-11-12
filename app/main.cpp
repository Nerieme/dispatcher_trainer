#include <iostream>
#include <string>
#include <filesystem>

#include "dispatcher/stl/SimulatorCore.hpp"
#include "dispatcher/stl/CommandResult.hpp"
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
              << "  find_index <name>           — найти индекс через std::optional\n"
              << "  set <name> <value>          — установить значение (optional)\n"
              << "  cmd <name> <value>          — команда оператора (variant + visit)\n"
              << "  save_log                    — сохранить журнал через std::filesystem\n"
              << "  alarms                      — тревожные события\n"
              << "  count_alarms                — количество тревожных событий\n"
              << "  log                         — исходный журнал\n"
              << "  log_sorted                  — отсортированная копия журнала\n"
              << "  stats                       — статистика по типам событий\n"
              << "  names                       — имена параметров\n"
              << "  lines                       — параметры name=value\n"
              << "  help                        — справка\n"
              << "  exit                        — выход\n";
}

SimulatorCore buildDemoCore() {
    SimulatorCore core;

    // Параметры тренажёра безопасности
    core.addParameter({"smoke_level",   12.0,  0.0,   60.0, "%"});
    core.addParameter({"temperature",   28.5, 18.0,   70.0, "C"});
    core.addParameter({"active_sensors", 2.0,  0.0,   10.0, "шт."});
    core.addParameter({"brigade_load",  40.0,  0.0,  100.0, "%"});

    // Журнал событий
    core.addEvent({1, EventType::Info,           "Тренажёр запущен",               1});
    core.addEvent({2, EventType::OperatorAction, "Оператор запросил статус",       2});
    core.addEvent({3, EventType::Warning,        "Датчик room A: задымление 45%",  5});
    core.addEvent({4, EventType::Alarm,          "Датчик room A: задымление 78%", 10});
    core.addEvent({5, EventType::Warning,        "Камера room D: нет сигнала",     4});
    core.addEvent({6, EventType::Info,           "Brigade A: выехала в build 1",   2});

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
        } else if (line.rfind("find ", 0) == 0) {
            std::string name = line.substr(5);
            auto p = core.findParameter(name);
            if (p) {
                std::cout << "Найден: " << p->name << " = " << p->value
                          << " " << p->unit << "\n";
            } else {
                std::cout << "Параметр не найден: " << name << "\n";
            }
        } else if (line.rfind("find_index ", 0) == 0) {
            std::string name = line.substr(11);
            auto idx = core.findParameterIndex(name);
            if (idx) {
                std::cout << "Индекс параметра \"" << name << "\": " << *idx << "\n";
            } else {
                std::cout << "Параметр не найден: " << name << "\n";
            }
        } else if (line.rfind("set ", 0) == 0) {
            // set <name> <value>
            std::size_t space = line.find(' ', 4);
            if (space == std::string::npos) {
                std::cout << "Формат: set <name> <value>\n";
                continue;
            }
            std::string name = line.substr(4, space - 4);
            double value = 0.0;
            try {
                value = std::stod(line.substr(space + 1));
            } catch (...) {
                std::cout << "Некорректное значение\n";
                continue;
            }
            if (core.setParameterValue(name, value)) {
                std::cout << "Значение изменено: " << name << "\n";
            } else {
                std::cout << "Параметр не найден: " << name << "\n";
            }
        } else if (line.rfind("cmd ", 0) == 0) {
            // cmd <name> <value>
            std::size_t space = line.find(' ', 4);
            if (space == std::string::npos) {
                std::cout << "Формат: cmd <name> <value>\n";
                continue;
            }
            std::string name = line.substr(4, space - 4);
            double value = 0.0;
            try {
                value = std::stod(line.substr(space + 1));
            } catch (...) {
                std::cout << "Некорректное значение\n";
                continue;
            }
            CommandResult result = core.applyOperatorCommand(name, value);
            std::cout << commandResultToText(result) << "\n";
        } else if (line == "save_log") {
            std::filesystem::path logPath = std::filesystem::path("logs") / "security_session.txt";
            try {
                core.saveTextLog(logPath);
                std::cout << "Журнал сохранён: " << logPath.string() << "\n";
            } catch (const std::exception& ex) {
                std::cout << "Ошибка сохранения: " << ex.what() << "\n";
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