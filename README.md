Лабораторная работа 13. Отладка, sanitizers и итоговая сборка модуля.
Вариант предметной области: диспетчер безопасности объекта.

## Дефект
Граница диапазона проверяется неверно: `applyOperatorCommand` использует
`>=` и `<=` вместо `>` и `<`, из-за чего значение ровно на границе
(min или max) ошибочно считается «вне нормы».

## Шаги воспроизведения
1. Собрать проект: `cmake --build build-debug`.
2. Запустить: `./build-debug/dispatcher_app`.
3. Ввести `status` — параметр `temperature` = 28.5, `max = 70.0`.
4. Ввести `cmd temperature 70`.
5. **Ожидаемый результат:** «Значение изменено: temperature».
6. **Фактический результат:** «Значение вне нормы: temperature».

## Место остановки в отладчике и причина
Breakpoint в SimulatorCore::applyOperatorCommand на строке if (newValue >= p.maxValue || ...). При cmd temperature 70 в Locals видно: newValue = 70.0, 
p.maxValue = 70.0. Условие 70.0 >= 70.0 истинно → уходим в CommandWarning. Причина — нестрогое сравнение на границе.

## Суть исправления
Условие заменено на newValue > p.maxValue || newValue < p.minValue. Граница стала допустимой. Добавлен тест test_value_on_boundary_is_accepted, который проверяет, что 70.0 и 18.0 дают CommandOk.

## Debug-сборка
```bash
cmake -S . -B build-debug -DCMAKE_BUILD_TYPE=Debug
cmake --build build-debug
ctest --test-dir build-debug --output-on-failure
```

## Сборка с sanitizers
```bash
cmake -S . -B build-sanitize -DCMAKE_BUILD_TYPE=Debug -DENABLE_SANITIZERS=ON
cmake --build build-sanitize
ctest --test-dir build-sanitize --output-on-failure
```

## Демонстрационный сценарий

1. Запустить ./build-debug/dispatcher_app.
2. Ввести status — проверить параметры.
3. Ввести cmd temperature 70 — значение ровно на границе.
4. Убедиться: «Значение изменено: temperature» (не «вне нормы»).
5. Ввести cmd temperature 99 — «Значение вне нормы: temperature».
6. Ввести cmd unknown 1 — «Ошибка команды: параметр не найден — unknown».
7. Ввести save_log — журнал сохранён в logs/security_session.txt.
8. Ввести exit — выход.

## Найденный дефект
В applyOperatorCommand использовалось >=/<= вместо >/<.
Значение ровно на границе (70.0 при max = 70.0) ошибочно отклонялось.
Исправлено: заменено на > и <. Добавлен тест test_value_on_boundary_is_accepted.