#pragma once

enum class SituationState {
    Normal,
    Warning,
    Emergency
};

enum class DispatcherCommand {
    None,
    ReduceAlerts,
    ReduceRiskLevel,
    RestoreConnection,
    ResetScenario
};

bool is_load_valid(double risk_level);

SituationState calculate_state(double risk_level,
                               bool sensors_connect_ok,
                               int number_of_alerts);

void print_state(SituationState state);

void print_status(double risk_level,
                  bool sensors_connect_ok,
                  int number_of_alerts,
                  SituationState state);

void apply_command(DispatcherCommand command,
                   double& risk_level,
                   bool& sensors_connect_ok,
                   int& number_of_alerts);