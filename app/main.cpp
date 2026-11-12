#include <iostream>
#include <memory>

#include "dispatcher/model/DispatcherModel.hpp"
#include "dispatcher/model/Room.hpp"
#include "dispatcher/model/Brigade.hpp"
#include "dispatcher/model/Alarm.hpp"
#include "dispatcher/model/Incident.hpp"
#include "dispatcher/model/ARHint.hpp"
#include "dispatcher/visual/ConsoleVisualRenderer.hpp"

using namespace dispatcher;

int main() {
    try {
        DispatcherModel model;

        // Комнаты
        model.addElement(std::make_unique<Room>("ROOM-A", "room A", "build 1", ElementStatus::Warning));
        model.addElement(std::make_unique<Room>("ROOM-D", "room D", "build 2", ElementStatus::Normal));

        // Бригады
        model.addElement(std::make_unique<Brigade>("BR-A", "Brigade A", "build 1", false));
        model.addElement(std::make_unique<Brigade>("BR-B", "Brigade B", "build 2", false));

        // Тревога
        model.addElement(std::make_unique<Alarm>("AL-01", "ROOM-A", Severity::High));

        // Инцидент
        model.addElement(std::make_unique<Incident>(
            "INC-01", "ROOM-A", "build 1", Severity::High));

        // AR-подсказка, привязанная к комнате A
        model.addElement(std::make_unique<ARHint>(
            "AR-01", "Проверьте комнату A, сработал датчик дыма", "ROOM-A", 70));

        model.printSummary();

        ConsoleVisualRenderer renderer;
        renderer.render(model.collectVisualDescriptors());

    } catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << '\n';
        return 1;
    }
    return 0;
}