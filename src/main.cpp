#include <iostream>
#include <string>

int calculate_startup_score(int tools_ready, int checks_passed) {
    return tools_ready * 10 + checks_passed;
}

int main() {
    const std::string project_name = "dispatcher_trainer_lab1";
    const int tools_ready = 4;
    const int checks_passed = 3;
    const int score = calculate_startup_score(tools_ready, checks_passed);
    std::cout << "Project: " << project_name << "\n";
    std::cout << "C++ standard: C++20" << "\n";
    std::cout << "Startup score: " << score << "\n";
    std::cout << "Environment is ready for the next labs." << "\n";
    return 0;
}