cmake_minimum_required(VERSION 3.22)
project(dispatcher_trainer_lab1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# =========================================================
# Библиотека ядра тренажёра
# =========================================================
add_library(dispatcher_core
    src/stl/Event.cpp
    src/stl/SimulatorCore.cpp
)

target_include_directories(dispatcher_core
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_features(dispatcher_core
    PUBLIC
        cxx_std_20
)

target_compile_options(dispatcher_core
    PRIVATE
        -Wall -Wextra -Wpedantic
)

# =========================================================
# Консольное приложение
# =========================================================
add_executable(dispatcher_app
    app/main.cpp
)

target_link_libraries(dispatcher_app
    PRIVATE
        dispatcher_core
)

target_compile_features(dispatcher_app
    PRIVATE
        cxx_std_20
)

target_compile_options(dispatcher_app
    PRIVATE
        -Wall -Wextra -Wpedantic
)

# =========================================================
# Тесты
# =========================================================
enable_testing()

add_executable(test_stl_core
    tests/test_stl_core.cpp
)
target_link_libraries(test_stl_core PRIVATE dispatcher_core)
target_compile_features(test_stl_core PRIVATE cxx_std_20)
add_test(NAME stl_core COMMAND test_stl_core)

add_executable(test_modern_cpp
    tests/test_modern_cpp.cpp
)
target_link_libraries(test_modern_cpp PRIVATE dispatcher_core)
target_compile_features(test_modern_cpp PRIVATE cxx_std_20)
add_test(NAME modern_cpp COMMAND test_modern_cpp)

# target_include_directories(dispatcher_trainer PRIVATE include)

# target_compile_options(dispatcher_trainer PRIVATE
#     -Wall
#     -Wextra
#     -Wpedantic
# )