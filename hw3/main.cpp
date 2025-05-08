#include "logger.h"
#include <cctype>
#include <string>

std::string to_lower(const std::string& str) {
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(),
                   [](unsigned char c) { return std::tolower(c); });
    return result;
}

SinkType parse_sink_type(const std::string& arg) {
    std::string lower_arg = to_lower(arg);
    if (lower_arg == "console") return SinkType::CONSOLE;
    if (lower_arg == "file") return SinkType::FILE;
    if (lower_arg == "none") return SinkType::NONE;
    throw std::invalid_argument("Invalid sink type: " + arg);
}

int main(int argc, char* argv[]) {
    try {
        SinkType type = SinkType::CONSOLE;
        if (argc > 1) {
            type = parse_sink_type(argv[1]);
        } else {
            std::cout << "No sink type provided, using default: CONSOLE.\n";
        }

        Logger::instance().set_sink(type);
        Logger::instance().log("Test message 1");
        Logger::instance().log("Test message 2");

    } catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << "\n";
        std::cerr << "Usage: ./logger [console|file|none]\n";
        return 1;
    }

    return 0;
}
