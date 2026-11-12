## Вариант

Диспетчер безопасности объекта.
Датчики по имени/зоне, «Норма, тревога, ошибка датчика», журнал — `logs/security_session.txt`.

## std::optional

Используется в:

- `SimulatorCore::findParameterIndex(const std::string&) const` — возвращает `std::optional<std::size_t>` вместо «магического» `-1`;
- `SimulatorCore::findParameter(const std::string&) const` — возвращает `std::optional<Parameter>`;
- `SimulatorCore::setParameterValue(...)` — использует результат `findParameterIndex`, при `std::nullopt` пишет событие «Параметр не найден» и возвращает `false`.

## std::variant

Результат команды оператора:
```cpp
using CommandResult = std::variant<CommandOk, CommandWarning, CommandError>;

CommandOk — значение успешно изменено;
CommandWarning — значение вышло за верхнюю границу;
CommandError — параметр не найден.
```

## std::visit

Преобразование в текст:
```cpp
inline std::string commandResultToText(const CommandResult& result) {
    return std::visit([](const auto& item) { return item.text; }, result);
}
```

## std::filesystem

Сохранение журнала:
```cpp
void SimulatorCore::saveTextLog(const std::filesystem::path& path) const;
```

1. каталог logs/ создаётся автоматически через std::filesystem::create_directories;
2. путь собирается через std::filesystem::path("logs") / "security_session.txt", а не конкатенацией строк;
3. формат — обычный текст, одна строка на событие.

## Smart pointers
В текущей версии не применяются. Все объекты (Parameter, Event)
хранятся по значению в std::vector внутри SimulatorCore. Композиция
покрывает все потребности учебного ядра, unique_ptr/shared_ptr
избыточны.

## Команды ЛР 8

find_index <name> — поиск индекса через std::optional;
set <name> <value> — установка значения (внутри optional);
cmd <name> <value> — команда оператора (variant + visit);
save_log — сохранение журнала через std::filesystem.

## Сборка и тесты

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
./build/dispatcher_trainer
ctest --test-dir build --output-on-failure
```

## 9. Контрольные вопросы

1. **`std::optional`** решает проблему «значение может отсутствовать» без магических значений.
2. **`-1` хуже `std::optional<size_t>`** — потому что `-1` может случайно совпасть с корректным значением после преобразования в `size_t`, а `optional` заставляет явно проверить наличие.
3. **`std::nullopt` vs ошибка** — это нормальное «нет значения», а не исключение; ошибка — это исключительная ситуация.
4. **`optional<Parameter>` vs индекс** — `optional<Parameter>` копирует объект; индекс легче, если дальше нужно менять объект в контейнере.
5. **`variant` хранит** одну из альтернатив, типобезопасно.
6. **`variant` безопаснее `type`-строки** — компилятор проверяет альтернативы, `std::visit` исчерпывающе обрабатывает все случаи.
7. **`std::visit`** — единый способ обойти variant без ручного `switch` по индексу.
8. **`filesystem`** — работа с путями, каталогами, проверка существования файлов.
9. **`filesystem::path`** — кроссплатформенный, умеет склеивать части через `/`, не требует ручной конкатенации.
10. **`unique_ptr` vs `shared_ptr`** — единоличное vs разделяемое владение.
11. **Не заменять всё на smart pointers** — для значений в `vector`/`map` они не нужны, композиция проще.
12. **Признаки необходимости smart pointer** — динамический полиморфизм, объект создаётся в куче и не должен копироваться, разделяемое владение.