#include <iostream>
#include "trainer/model.hpp"

using namespace std;

void addEvent(EventLog& log,
              const string& severity,
              const string& source,
              const string& message) {
    log.push_back({severity, source, message});
}

void printEventLog(const EventLog& log) {
    if (log.empty()) {
        cout << "Журнал пуст.\n";
        return;
    }
    cout << "Журнал событий:\n";
    for (const EventRecord& event : log) {
        cout << "[" << event.severity << "] " << event.source << ": " << event.message << "\n";
    }
}

void checkParametersAndLogWarnings(const ParameterList& parameters,
                                   EventLog& log) {
    for (const ParameterRecord& p : parameters) {
        if (!isInRange(p)) {
            addEvent(log, "WARN", p.name, "значение вне допустимого диапазона");
        }
    }
}