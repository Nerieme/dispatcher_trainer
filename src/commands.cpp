#include "trainer/commands.hpp"

#include <iostream>
#include <sstream>

using namespace std;

vector<string> splitCommand(const string& line) {
    istringstream input(line);
    vector<string> tokens;
    string token;
    while (input >> token) {
        tokens.push_back(token);
    }
    return tokens;
}

void printHelp() {
    cout << "Доступные команды:\n"
         << "  help                          - показать этот список\n"
         << "  status                        - показать текущие параметры\n"
         << "  set <parameter> <value>       - изменить значение параметра\n"
         << "  event <severity> <source>     - добавить событие вручную\n"
         << "  log                           - показать журнал событий\n"
         << "  camera                        - показать статус камеры\n"
         << "  visual                        - показать visual/AR-слои\n"
         << "  exit                          - завершить работу\n";
}