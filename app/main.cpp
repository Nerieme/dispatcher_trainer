#include <iostream>

#include "trainer_logic.hpp"

using namespace std;

int read_menu_item() {
    cout << "\n1. Показать состояние\n";
    cout << "2. Изменить уровень риска\n";
    cout << "3. Уменьшить количество тревог\n";
    cout << "4. Снизить уровень риска\n";
    cout << "5. Восстановить связь\n";
    cout << "6. Перезапустить сценарий\n";
    cout << "7. Пересчитать состояние\n";
    cout << "0. Выход\n";
    cout << "Выбор: ";

    int item = -1;
    cin >> item;
    return item;
}

int main() {
    double risk_level = 62.0;
    bool sensors_connect_ok = true;
    int number_of_alerts = 1;
    SituationState state = calculate_state(risk_level, sensors_connect_ok, number_of_alerts);

    bool running = true;
    while (running) {
        int item = read_menu_item();
        switch (item) {
            case 1:
                print_status(risk_level, sensors_connect_ok, number_of_alerts, state);
                break;
            case 2:
                cout << "Новый уровень риска, %: ";
                cin >> risk_level;
                if (!is_load_valid(risk_level)) {
                    cout << "Ошибка: значение должно быть от 0 до 100.\n";
                    risk_level = 100.0;
                }
                break;
            case 3:
                apply_command(DispatcherCommand::ReduceAlerts, risk_level, sensors_connect_ok, number_of_alerts);        
                break;
            case 4:
                apply_command(DispatcherCommand::ReduceRiskLevel, risk_level, sensors_connect_ok, number_of_alerts);        
                break;
            case 5:
                apply_command(DispatcherCommand::RestoreConnection, risk_level, sensors_connect_ok, number_of_alerts);        
                break;
            case 6:
                apply_command(DispatcherCommand::ResetScenario, risk_level, sensors_connect_ok, number_of_alerts);        
                break;
            case 7:
                state = calculate_state(risk_level, sensors_connect_ok, number_of_alerts);
                cout << "Состояние пересчитано.\n";
                break;
            case 0:
                running = false;
                break;
            default:
                std::cout << "Неизвестный пункт меню.\n";
        }
    }
}