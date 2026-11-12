# dispatcher_trainer_lab6

## Вариант

**Диспетчер безопасности объекта.** Здание с комнатами A–F,
две бригады (Brigade A — build 1, Brigade B — build 2).
Параметры: уровень задымления, температура, число активных датчиков,
загрузка бригады.

## Схема в `diagrams/model.mmd`

## Что реализовано

- `Range<T>` — обобщённый диапазон с проверкой `min <= max` в конструкторе.
- `Parameter<T>` — типизированный параметр с id, label, unit, value.
- `ICheckRule` — общий интерфейс правила проверки.
- `RangeRule<T>` — шаблонное правило, наследующее `ICheckRule`.
- `VisualDescriptor` + `colorBySeverity` — DTO для будущего клиента/AR.
- `concept OrderedValue` — ограничение шаблона в C++20.

## Функциональный и классовый шаблоны

- Классовый: `Range<T>`, `Parameter<T>`, `RangeRule<T>`.
- Функциональный: `Range<T>::contains`, `colorBySeverity` (inline, не шаблон, но
  используется как общая функция), `RangeRule<T>::check` — шаблонный метод.

## Инстанцирования (≥ 2 разных типов)

- `Parameter<double>` — `smokeLevel`, `temperature`.
- `Parameter<int>` — `activeSensors`, `brigadeLoad`.

## Контрольные вопросы

1. Шаблон функции — обобщённая функция, компилятор создаёт конкретные версии под типы; обычная функция — одна на всё.

2. Шаблон класса полезен, когда одинаковые поля/методы нужны для разных типов (Parameter, Range).

3. Инстанцирование — генерация компилятором конкретной версии шаблона под конкретный тип.

4. Определения в заголовках — компилятор должен видеть тело шаблона при инстанцировании; иначе линковщик не найдёт код.

5. Шаблоны — compile-time, виртуальные функции — runtime. Шаблоны хороши для контейнеров/правил, виртуальные — для расширяемых иерархий.

6. Range<T> убирает дублирование: одно определение диапазона для всех типов.

7. Concepts дают понятные ошибки компиляции, если тип не поддерживает нужные операции (например, <=).

8. Parameter<double> и Parameter<int> — разные конкретные типы, потому что T — часть типа после инстанцирования.

9. RangeRule<T> + ICheckRule — шаблон наследует общий интерфейс, разные инстанцирования хранятся в vector<unique_ptr<ICheckRule>>.

10. DTO без GUI/AR — чтобы модель не зависела от будущих слоёв; клиент сам решит, как отрисовать VisualDescriptor.

11. clangd и шаблоны — если clangd ругается на корректный код, нужно CMake: Configure и clangd: Restart language server; проверить compile_commands.json.

12. В CodeLLDB внутри RangeRule<T>::check() видно: parameter_ (id, label, value, unit), normalRange_ (min, max), problemMessage_, а в Call Stack — какое именно инстанцирование (RangeRule<double>::check или RangeRule<int>::check).

## Проверка

≥ 2 инстанцирования (Parameter<double>, Parameter<int>)  => 4 параметра, 2 типа
≥ 1 классовый шаблон	                                   => Range<T>, Parameter<T>, RangeRule<T>
≥ 1 функциональный шаблон или шаблонный метод            => RangeRule<T>::check() — шаблонный метод
Range<T>
Обобщённое правило для нескольких типов                  => RangeRule<double> и RangeRule<int>
≥ 3 параметра диспетчерской предметной области           => 4 параметра
DTO для будущей визуализации                             => VisualDescriptor
Тесты через CTest	                                       => test_template_rules
Сборка в Dev Container / CodeLLDB                        => CMake + launch.json
README с командами, вариантом и Mermaid