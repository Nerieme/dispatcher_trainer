cmake_minimum_required(VERSION 3.22)
project(dispatcher_trainer_lab1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# =========================================================
# Санитайзеры (AddressSanitizer + UBSan)
# =========================================================
option(ENABLE_SANITIZERS "Enable AddressSanitizer and UBSan" OFF)

function(enable_project_sanitizers target_name)
    if (ENABLE_SANITIZERS AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        target_compile_options(${target_name} PRIVATE
            -fsanitize=address,undefined
            -fno-omit-frame-pointer
            -g
        )
        target_link_options(${target_name} PRIVATE
            -fsanitize=address,undefined
        )
    endif()
endfunction()

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

add_executable(dispatcher_tests
    tests/test_core.cpp
)

target_link_libraries(dispatcher_tests
    PRIVATE
        dispatcher_core
)

target_compile_features(dispatcher_tests
    PRIVATE
        cxx_std_20
)

target_compile_options(dispatcher_tests
    PRIVATE
        -Wall -Wextra -Wpedantic
)

add_test(NAME dispatcher_tests COMMAND dispatcher_tests)

# =========================================================
# Применение санитайзеров к целям (после их создания)
# =========================================================
enable_project_sanitizers(dispatcher_core)
enable_project_sanitizers(dispatcher_app)
enable_project_sanitizers(dispatcher_tests)