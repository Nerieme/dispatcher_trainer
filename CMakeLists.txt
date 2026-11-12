cmake_minimum_required(VERSION 3.22)
project(dispatcher_trainer_lab1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ---------- Основное приложение ----------
add_executable(dispatcher_trainer
    app/main.cpp
    src/stl/Event.cpp
    src/stl/SimulatorCore.cpp
)

target_include_directories(dispatcher_trainer PRIVATE include)
target_compile_options(dispatcher_trainer PRIVATE -Wall -Wextra -Wpedantic)

# ---------- Тесты ----------
enable_testing()

add_executable(test_stl_core
    tests/test_stl_core.cpp
    src/stl/Event.cpp
    src/stl/SimulatorCore.cpp
)
target_include_directories(test_stl_core PRIVATE include)
target_compile_options(test_stl_core PRIVATE -Wall -Wextra -Wpedantic)

add_test(NAME stl_core COMMAND test_stl_core)

# target_include_directories(dispatcher_trainer PRIVATE include)

# target_compile_options(dispatcher_trainer PRIVATE
#     -Wall
#     -Wextra
#     -Wpedantic
# )