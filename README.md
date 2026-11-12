# dispatcher_trainer_lab5

## Вариант

**Диспетчер безопасности объекта (пожаробезопасность).**

Здание с шестью комнатами (A–F) обслуживают две бригады:
Brigade A (build 1) — комнаты A, B, C;
Brigade B (build 2) — комнаты D, E, F.

## Схема в `diagrams/model.mmd`

## В коде есть интерфейс, не менее 4 производных классов и контейнер

Интерфейсы: ISystemElement, IVisualDescriptorProvider.

Производные классы: Room, Brigade, Alarm, Incident, ARHint — 5 классов.

Контейнер: std::vector<std::unique_ptr<ISystemElement>> elements_; в DispatcherModel.