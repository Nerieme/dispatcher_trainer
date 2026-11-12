#include "dispatcher/model/DispatcherModel.hpp"
#include "dispatcher/model/Room.hpp"
#include "dispatcher/model/Brigade.hpp"
#include "dispatcher/model/ARHint.hpp"

#include <cassert>
#include <iostream>

using namespace dispatcher;

int main() {
    DispatcherModel model;
    model.addElement(std::make_unique<Room>("ROOM-A", "room A", "build 1", ElementStatus::Normal));
    model.addElement(std::make_unique<Brigade>("BR-A", "Brigade A", "build 1"));
    model.addElement(std::make_unique<ARHint>("AR-01", "hint", "ROOM-A", 30));

    assert(model.size() == 3);

    auto descriptors = model.collectVisualDescriptors();
    assert(descriptors.size() == 3);

    for (const auto& d : descriptors) {
        assert(!d.anchorId.empty());
        assert(!d.label.empty());
    }

    std::cout << "All tests passed\n";
    return 0;
}