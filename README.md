## Название работы и выбранный вариант предметной области

Лабораторная работа 11. Воспроизводимая учебная среда.
Вариант предметной области: диспетчер безопасности объекта.

## Краткое описание того, зачем проекту нужна воспроизводимая среда

В предыдущих лабораторных работах проект собирался через CMake на конкретном компьютере. При переносе на другую машину могли возникнуть проблемы:

1. разные версии g++/clang++ дают разное поведение для std::optional, std::variant, concepts;
2. разная версия CMake может по-разному обрабатывать target_compile_features и target_include_directories;
3. разный набор установленных пакетов (build-essential, gdb, git);
4. настройки VS Code (launch.json, settings.json) у каждого свои.

Воспроизводимая среда через Docker Dev Container решает эти проблемы:

1. все студенты и преподаватель работают в одинаковой версии и одинаковом наборе инструментов;
2. сборка выполняется по одному сценарию — cmake -S . -B build-debug, cmake --build build-debug;
3. .devcontainer/ хранится в Git, поэтому среда передаётся вместе с проектом;
4. README содержит единые команды для devcontainer и Linux VM.

Это делает проект воспроизводимым: преподаватель открывает репозиторий в контейнере и получает тот же результат, что и автор.

## Текст Dockerfile и пояснение пакетов

```dockerfile
FROM mcr.microsoft.com/devcontainers/cpp:1-debian-12

RUN apt-get update \
    && export DEBIAN_FRONTEND=noninteractive \
    && apt-get install -y --no-install-recommends \
        cmake ninja-build git clang clangd clang-format lldb gdb \
        pkg-config ca-certificates \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /workspaces/dispatcher_trainer
```

1. mcr.microsoft.com/devcontainers/cpp:1-debian-12	- 
базовый образ Microsoft Dev Containers на Debian 12 (bookworm). Уже содержит vscode-пользователя, git, sudo, типичные настройки devcontainer
2. cmake - сборка проекта
3. ninja-build - генератор Ninja (в devcontainer.json — "cmake.generator": "Ninja")
4. git - история, коммиты
5. clang - компилятор clang++ (проверяется в postCreateCommand)
6. clangd - language server для VS Code (проверяется в postCreateCommand)
7. clang-format - форматирование кода
8. lldb - отладчик, на котором работает CodeLLDB (проверяется в postCreateCommand)
9. gdb - альтернативный отладчик из консоли
10. pkg-config, ca-certificates - на будущее
11. WORKDIR /workspaces/dispatcher_trainer — рабочая папка, совпадает с workspaceFolder в devcontainer.json.

## Текст devcontainer.json и пояснение
```json
{
  "name": "C++20 Dispatcher Trainer",
  "build": {
    "dockerfile": "Dockerfile",
    "context": "."
  },
  "customizations": {
    "vscode": {
      "extensions": [
        "ms-vscode.cmake-tools",
        "llvm-vs-code-extensions.vscode-clangd",
        "vadimcn.vscode-lldb"
      ],
      "settings": {
        "C_Cpp.intelliSenseEngine": "disabled",
        "cmake.configureOnOpen": true,
        "cmake.generator": "Ninja"
      }
    }
  },
  "postCreateCommand": "cmake --version && clang++ --version && clangd --version && lldb --version",
  "remoteUser": "vscode"
}
```

1. name - отображается в левом нижнем углу VS Code
2. build.dockerfile - путь к Dockerfile
3. build.context - контекст сборки — корень проекта
4. customizations.vscode.extensions - рекомендованные расширения: CMake Tools, clangd, CodeLLDB
5. customizations.vscode.settings - отключает встроенный IntelliSense (используем clangd), включает авто-Configure CMake, задаёт Ninja
6. postCreateCommand - после создания контейнера проверяет версии cmake, clang++, clangd, lldb — если что-то не установлено, контейнер сообщит об ошибке
7. remoteUser - пользователь внутри контейнера — vscode

## Проверка версий инструментов внутри контейнера

cmake --version
clang++ --version
clangd --version
lldb --version
ninja --version
git --version

## Команды CMake-сборки и запуска внутри контейнера

```bash
rm -rf build-debug
cmake -S . -B build-debug -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
cmake --build build-debug
./build-debug/dispatcher_app
```

Release-сборка:
```bash
cmake -S . -B build-release -DCMAKE_BUILD_TYPE=Release -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
cmake --build build-release
./build-release/dispatcher_app
```

Тесты:
```bash
ctest --test-dir build-debug --output-on-failure
```

## Инструкция запуска

### Вариант 1 — Docker Dev Container (основной)

1. Установить Docker Desktop и расширение VS Code **Dev Containers**.
2. Открыть папку проекта в VS Code.
3. Command Palette → **Dev Containers: Reopen in Container**.
4. Дождаться сборки контейнера. `postCreateCommand` автоматически проверит
   версии `cmake`, `clang++`, `clangd`, `lldb`.
5. В терминале контейнера собрать и запустить:

```bash
cmake -S . -B build-debug -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
cmake --build build-debug
./build-debug/dispatcher_app
```

### Вариант 2 — Linux VM VirtualBox (альтернативный)

```bash
sudo apt update
sudo apt install -y build-essential clang clangd lldb cmake ninja-build git
cmake -S . -B build-debug -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
cmake --build build-debug
./build-debug/dispatcher_app
```