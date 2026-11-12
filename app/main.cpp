#include <iostream>
#include <string>
#include <vector>

#include "trainer/model.hpp"
#include "trainer/log.hpp"
#include "trainer/commands.hpp"

using namespace std;

int main() {
    ParameterList parameters = {
        {"smoke_level", 25, 0, 100, "%"},
        {"active_sensors", 16, 0, 20, "ed."},
        {"camera_status", 0, 0, 2, "code."},
        {"alarm_level", 0, 0, 3, "yp."}
    };

    EventLog log;
    addEvent(log, "INFO", "system", "система запущена");

    cout << "Введите help для списка команд.\n";

    string line;
    bool running = true;

    while (running) {
        cout << "dispatcher> ";
        getline(cin, line);
        vector<string> tokens = splitCommand(line);

        if (tokens.empty()) {
            continue;
        }

        const string& command = tokens[0];

        if (command == "status") {
            printParameters(parameters);
        } else if (command == "log") {
            printEventLog(log);
        } else if (command == "visual") {
            printVisualLayers(visualLayerNames());
        } else if (command == "exit") {
            addEvent(log, "INFO", "system", "тренажер завершен");
            running = false;
        } else if (command == "set") {
            if (tokens.size() != 3) {
                cout << "Формат: set <parameter> <value>\n";
                addEvent(log, "ERROR", "command", "неверный формат set");
                continue;
            }

            const string& name = tokens[1];
            int value = stod(tokens[2]);

            if (setParameter(parameters, name, value)) {
                addEvent(log, "INFO", name, "значение изменено");
                checkParametersAndLogWarnings(parameters, log);
            } else {
                cout << "Параметр не найден: " << name << "\n";
                addEvent(log, "ERROR", name, "параметр не найден");
            }
        } else if (command == "event") {
            if (tokens.size() < 3) {
                std::cout << "Формат: event <severity> <message>\n";
                addEvent(log, "ERROR", "command", "неверный формат event");
                continue;
            }

            const string& severity = tokens[1];
            string message;

            for (size_t i = 2; i < tokens.size(); ++i) {
                if (i > 2) message += " ";
                message += tokens[i];
            }

            addEvent(log, severity, "operator_ack", message);
        } else if (command == "help") {
            printHelp();
            addEvent(log, "INFO", "command", "запрошена справка");
        } else if (command == "camera") {
            printCameraStatus(parameters);
        } else {
            cout << "Неизвестная команда. Введите help.\n";
            addEvent(log, "ERROR", "command", "неизвестная команда: " + command);
        }
    }
    return 0;
}