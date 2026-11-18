#include <cppunit/TestCase.h>
#include <cppunit/TestFixture.h>
#include <cppunit/TestCaller.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestResult.h>
#include <cppunit/TextOutputter.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/extensions/HelperMacros.h>

#include <filesystem>
#include <fstream>
#include <string>

#include "dispatcher/stl/CommandResult.hpp"
#include "dispatcher/stl/Event.hpp"
#include "dispatcher/stl/Parameter.hpp"
#include "dispatcher/stl/SimulatorCore.hpp"

using namespace dispatcher;

class SimulatorCoreTest : public CppUnit::TestFixture {
private:
    SimulatorCore* core_;

public:
    void setUp() override {
        core_ = new SimulatorCore();
        core_->addParameter({"smoke_level",   12.0,  0.0,   60.0, "%"});
        core_->addParameter({"temperature",   28.5, 18.0,   70.0, "C"});
        core_->addParameter({"active_sensors", 2.0,  0.0,   10.0, "шт."});
        core_->addParameter({"brigade_load",  40.0,  0.0,  100.0, "%"});
        core_->rebuildParameterIndex();

        core_->addEvent({1, EventType::Info,           "Тренажёр запущен",               1});
        core_->addEvent({2, EventType::OperatorAction, "Оператор запросил статус",       2});
        core_->addEvent({3, EventType::Warning,        "Датчик room A: задымление 45%",  5});
        core_->addEvent({4, EventType::Alarm,          "Датчик room A: задымление 78%", 10});
        core_->addEvent({5, EventType::Warning,        "Камера room D: нет сигнала",     4});
        core_->addEvent({6, EventType::Info,           "Brigade A: выехала в build 1",   2});
    }

    void tearDown() override {
        delete core_;
        core_ = nullptr;
    }

    // ---------- ТК-01: Установка значения в допустимом диапазоне ----------

    void testSetValueInRange() {
        // Шаг 1: set temperature 40
        CPPUNIT_ASSERT(core_->setParameterValue("temperature", 40.0));

        // Шаг 2: status — проверяем значение
        auto p = core_->findParameter("temperature");
        CPPUNIT_ASSERT(p.has_value());
        CPPUNIT_ASSERT_EQUAL(40.0, p->value);
        CPPUNIT_ASSERT_EQUAL(std::string("C"), p->unit);
    }

    // ---------- ТК-02: Команда оператора с выходом за верхнюю границу ----------

    void testOperatorCommandOutOfRange() {
        // Шаг 1: cmd temperature 99
        CommandResult result = core_->applyOperatorCommand("temperature", 99.0);

        // Ожидаемый результат: CommandWarning
        CPPUNIT_ASSERT(std::holds_alternative<CommandWarning>(result));

        // Проверяем текст
        CPPUNIT_ASSERT(commandResultToText(result) ==
                       "Значение вне нормы: temperature");
    }

    // ---------- ТК-03: Поиск несуществующего параметра ----------

    void testFindMissingParameter() {
        // Шаг 1: find speed
        auto found = core_->findParameter("speed");
        CPPUNIT_ASSERT(!found.has_value());

        auto index = core_->findParameterIndex("speed");
        CPPUNIT_ASSERT(!index.has_value());
    }

    // ---------- ТК-04: Просмотр и подсчёт тревожных событий ----------

    void testViewAndCountAlarmEvents() {
        // В журнале 2 тревожных события: Warning (id=3) и Alarm (id=4)
        const auto alarms = core_->filterAlarmEvents();
        CPPUNIT_ASSERT_EQUAL(std::size_t(3), alarms.size());

        CPPUNIT_ASSERT_EQUAL(std::size_t(3), core_->countAlarmEvents());
    }

    // ---------- ТК-05: Сохранение журнала в файл ----------

    void testSaveLogToFile() {
        const std::filesystem::path logPath =
            std::filesystem::path("logs") / "security_session_test.txt";

        // Удаляем файл, если он остался от предыдущего запуска
        std::filesystem::remove(logPath);

        // Шаг 1: save_log
        core_->saveTextLog(logPath);

        // Шаг 2: проверяем, что файл создан
        CPPUNIT_ASSERT(std::filesystem::exists(logPath));

        // Проверяем, что в файле 6 строк
        std::ifstream in(logPath);
        CPPUNIT_ASSERT(in.is_open());

        std::string line;
        int lineCount = 0;
        while (std::getline(in, line)) {
            ++lineCount;
        }
        CPPUNIT_ASSERT_EQUAL(6, lineCount);

        // Убираем файл после теста
        in.close();
        std::filesystem::remove(logPath);
    }

    // ---------- Дополнительные тесты (для полноты) ----------

    void testValueOnBoundaryIsAccepted() {
        CommandResult atMax = core_->applyOperatorCommand("temperature", 70.0);
        CPPUNIT_ASSERT(std::holds_alternative<CommandOk>(atMax));

        CommandResult atMin = core_->applyOperatorCommand("temperature", 18.0);
        CPPUNIT_ASSERT(std::holds_alternative<CommandOk>(atMin));
    }

    void testFindExistingParameter() {
        auto found = core_->findParameter("temperature");
        CPPUNIT_ASSERT(found.has_value());
        CPPUNIT_ASSERT_EQUAL(28.5, found->value);

        auto index = core_->findParameterIndex("temperature");
        CPPUNIT_ASSERT(index.has_value());
    }

    void testSortedLogIsCopy() {
        auto sorted = core_->sortedLog();
        CPPUNIT_ASSERT_EQUAL(std::size_t(6), sorted.size());

        // Наивысший приоритет — Alarm (id=4, prio=10)
        CPPUNIT_ASSERT_EQUAL(std::size_t(4), sorted[0].id);

        // Исходный журнал не изменился
        CPPUNIT_ASSERT_EQUAL(std::size_t(1), core_->events()[0].id);
    }

    void testEventStatistics() {
        auto stats = core_->eventStatistics();
        CPPUNIT_ASSERT_EQUAL(std::size_t(2), stats.at(EventType::Info));
        CPPUNIT_ASSERT_EQUAL(std::size_t(1), stats.at(EventType::OperatorAction));
        CPPUNIT_ASSERT_EQUAL(std::size_t(2), stats.at(EventType::Warning));
        CPPUNIT_ASSERT_EQUAL(std::size_t(1), stats.at(EventType::Alarm));
    }

    // ---------- Формирование TestSuite ----------

    CPPUNIT_TEST_SUITE(SimulatorCoreTest);
    CPPUNIT_TEST(testSetValueInRange);
    CPPUNIT_TEST(testOperatorCommandOutOfRange);
    CPPUNIT_TEST(testFindMissingParameter);
    CPPUNIT_TEST(testViewAndCountAlarmEvents);
    CPPUNIT_TEST(testSaveLogToFile);
    CPPUNIT_TEST(testValueOnBoundaryIsAccepted);
    CPPUNIT_TEST(testFindExistingParameter);
    CPPUNIT_TEST(testSortedLogIsCopy);
    CPPUNIT_TEST(testEventStatistics);
    CPPUNIT_TEST_SUITE_END();
};

// ---------- main с TestRunner ----------

int main() {
    CppUnit::TextUi::TestRunner runner;
    runner.addTest(SimulatorCoreTest::suite());
    bool wasSuccessful = runner.run();
    return wasSuccessful ? 0 : 1;
}