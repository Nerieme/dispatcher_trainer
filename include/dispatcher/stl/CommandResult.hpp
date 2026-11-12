#pragma once

#include <string>
#include <variant>

namespace dispatcher {

struct CommandOk {
    std::string text;
};

struct CommandWarning {
    std::string text;
};

struct CommandError {
    std::string text;
};

using CommandResult = std::variant<CommandOk, CommandWarning, CommandError>;

inline std::string commandResultToText(const CommandResult& result) {
    return std::visit([](const auto& item) { return item.text; }, result);
}

} // namespace dispatcher