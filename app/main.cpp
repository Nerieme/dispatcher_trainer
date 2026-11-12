#include <iostream>
#include <string>

#include "dispatcher/DispatchCenter.hpp"
#include "dispatcher/EventLog.hpp"

namespace {

// Комнаты A, B, C обслуживает Brigade A которая базируется в build 1.
// Комнаты D, E, F обслуживает Brigade B которая базируется в build 2.

int unitForRoom(const std::string& room) {
    if (room == "room A" || room == "room B" || room == "room C") {
        return 1;
    }
    if (room == "room D" || room == "room E" || room == "room F") {
        return 2;
    }
    return 0;
}

}

int main() {
    try {
        dispatcher::EventLog log{"dispatch.log"};
        dispatcher::DispatchCenter center{log};
 
        center.addUnit(dispatcher::DispatchUnit{1, "Brigade A", "build 1"});
        center.addUnit(dispatcher::DispatchUnit{2, "Brigade B", "build 2"});
 
        center.addIncident(dispatcher::Incident{
            100,
            "room A",
            "build 1",
            dispatcher::Severity::High,
            dispatcher::VisualMarker::Alert
        });
        center.addIncident(dispatcher::Incident{
            101,
            "room E",
            "build 2",
            dispatcher::Severity::Critical,
            dispatcher::VisualMarker::Evacuation
        });
 
        center.printState();

        const int incident1Unit = unitForRoom("room A");
        if (incident1Unit != 0) {
            center.assign(incident1Unit, 100);
        }
        const int incident2Unit = unitForRoom("room E");
        if (incident2Unit != 0) {
            center.assign(incident2Unit, 101);
        }

        center.printState();

        center.closeIncident(100);
        center.closeIncident(101);

        center.printState();

    } catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << '\n';
        return 1;
    }
    return 0;
}