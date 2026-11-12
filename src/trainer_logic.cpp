#include "trainer_logic.hpp"
#include <iostream>

using namespace std;

bool is_load_valid(double risk_level) {
    return risk_level >= 0.0 && risk_level <= 100.0;
}

SituationState calculate_state(double risk_level,
                               bool sensors_connect_ok,
                               int number_of_alerts) {
    if (!sensors_connect_ok || number_of_alerts >= 3 || risk_level >= 90.0) {
        return SituationState::Emergency;
    }
    if (number_of_alerts > 0 || risk_level >= 75.0) {
        return SituationState::Warning;
    }
    return SituationState::Normal;
}

void print_state(SituationState state) {
    switch (state) {
        case SituationState::Normal: std::cout << "Норма"; break;
        case SituationState::Warning: std::cout << "Предупреждение"; break;
        case SituationState::Emergency: std::cout << "Аварийная ситуация"; break;
    }
}

void print_status(double risk_level,
                  bool sensors_connect_ok,
                  int number_of_alerts,
                  SituationState state) {
    cout << "\n[Уровень риска: " << risk_level << "%]\n";
    if (!sensors_connect_ok) cout << "[Связь с датчиками потеряна!]\n";
    cout << "[Связь с датчиками установлена]\n";
    cout << "[Количество активных тревог: " << number_of_alerts << "]\n";
    cout << "[Статус угрозы: ";
    print_state(state);
    cout << "]\n";
}

void apply_command(DispatcherCommand command,
                   double& risk_level,
                   bool& sensors_connect_ok,
                   int& number_of_alerts) {
    switch (command) {
        case DispatcherCommand::ReduceAlerts:
            if (number_of_alerts > 0) {
                --number_of_alerts;
            }
            break;
        case DispatcherCommand::ReduceRiskLevel:
            risk_level -= 10.0;
            if (risk_level < 0.0) {
                risk_level = 0.0;
            }
            break;
        case DispatcherCommand::RestoreConnection:
            sensors_connect_ok = true;
            break;
        case DispatcherCommand::ResetScenario:
            risk_level = 50.0;
            sensors_connect_ok = true;
            number_of_alerts = 0;
            break;
        case DispatcherCommand::None:
            break;
    }
}