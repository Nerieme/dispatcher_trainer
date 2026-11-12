#pragma once

#include <filesystem>
#include <fstream>
#include <string_view>

namespace dispatcher {

class EventLog {
public:
    explicit EventLog(const std::filesystem::path& path);
    ~EventLog();

    EventLog(const EventLog&) = delete;
    EventLog& operator=(const EventLog&) = delete;
    EventLog(EventLog&&) = delete;
    EventLog& operator=(EventLog&&) = delete;

    void write(std::string_view message);

private:
    std::ofstream out_;
};

}