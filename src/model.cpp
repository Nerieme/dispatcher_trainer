#include "trainer/model.hpp"
#include <iostream>

using namespace std;

const array<string, 5>& visualLayerNames() {
    static const array<string, 5> layers = {
        "map", // базовая карта диспетчерской зоны
        "routes", // маршруты и связи датчиков
        "sensors", // контролируемые датчики
        "alarms", // тревоги, предупреждения и статусы
        "ar_markers" // будущие элементы дополненной реальности
    };
    return layers;
}

void printParameters(const ParameterList& parameters) {
    cout << "Параметры диспетчерской ситуации:\n";
    for (const ParameterRecord& p : parameters) {
        cout << "- " << p.name << " = " << p.value << " " << p.unit << " [" << p.minValue << ".." << p.maxValue << "]\n";
    }
}

bool setParameter(ParameterList& parameters,
                  const string& name,
                  int newValue) {
    for (ParameterRecord& p : parameters) {
        if (p.name == name) {
            p.value = newValue;
            return true;
        }
    }
    return false;
}

bool isInRange(const ParameterRecord& parameter) {
    return parameter.value >= parameter.minValue &&
           parameter.value <= parameter.maxValue;
}

void printVisualLayers(const array<string, 5>& layers) {
    cout << "Доступные visual/AR-слои:\n";
    for (const string& layer : layers) {
        cout << "- " << layer << "\n";
    }
}

void printCameraStatus(const ParameterList& parameters) {
    for (const ParameterRecord& p : parameters) {
        if (p.name == "camera_status") {
            if (p.value == 0) {
                cout << "- status: Turn ON\n";
            } else if (p.value == 1) {
                cout << "- status: Signal lost\n";
            } else {
                cout << "- status: Turn OFF\n";
            }
        }
    }
}