cmake_minimum_required(VERSION 3.22)
project(dispatcher_trainer_lab1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ---------- Основное приложение ----------
add_executable(dispatcher_trainer
    app/main.cpp
    src/model/CommonTypes.cpp
    src/model/Room.cpp
    src/model/Brigade.cpp
    src/model/Alarm.cpp
    src/model/Incident.cpp
    src/model/ARHint.cpp
    src/model/DispatcherModel.cpp
    src/visual/ConsoleVisualRenderer.cpp
)

target_include_directories(dispatcher_trainer PRIVATE include)
target_compile_options(dispatcher_trainer PRIVATE -Wall -Wextra -Wpedantic)

# ---------- Тесты ----------
add_executable(dispatcher_tests
    tests/test_model.cpp
    src/model/CommonTypes.cpp
    src/model/Room.cpp
    src/model/Brigade.cpp
    src/model/Alarm.cpp
    src/model/Incident.cpp
    src/model/ARHint.cpp
    src/model/DispatcherModel.cpp
)
target_include_directories(dispatcher_tests PRIVATE include)
target_compile_options(dispatcher_tests PRIVATE -Wall -Wextra -Wpedantic)

enable_testing()
add_test(NAME model_smoke COMMAND dispatcher_tests)

# target_include_directories(dispatcher_trainer PRIVATE include)

# target_compile_options(dispatcher_trainer PRIVATE
#     -Wall
#     -Wextra
#     -Wpedantic
# )