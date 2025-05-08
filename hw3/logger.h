#pragma once
#include <iostream>
#include <fstream>
#include <memory>
#include <string>
#include <algorithm>

enum class SinkType { CONSOLE, FILE, NONE };

struct LogSink {
    virtual void write(const std::string& msg) = 0;
    virtual ~LogSink() = default;
};

class ConsoleSink : public LogSink {
public:
    void write(const std::string& msg) override {
        std::cout << "[Console] " << msg << std::endl;
    }
};

class FileSink : public LogSink {
public:
    void write(const std::string& msg) override {
        std::ofstream ofs("app.log", std::ios::app);
        if (ofs.is_open()) {
            ofs << "[File] " << msg << std::endl;
        }
    }
};

class NullSink : public LogSink {
public:
    void write(const std::string& msg) override {
    }
};

class Logger {
public:
    static Logger& instance() {
        static Logger logger;
        return logger;
    }

    void set_sink(SinkType type) {
        switch (type) {
            case SinkType::CONSOLE:
                sink_ = std::make_unique<ConsoleSink>();
                std::cout << "Sink set to CONSOLE.\n";
                break;
            case SinkType::FILE:
                sink_ = std::make_unique<FileSink>();
                std::cout << "Sink set to FILE (app.log).\n";
                break;
            case SinkType::NONE:
                sink_ = std::make_unique<NullSink>();
                std::cout << "Sink set to NONE.\n";
                break;
            default:
                std::cerr << "Unknown sink type.\n";
                break;
        }
    }

    void log(const std::string& msg) {
        if (sink_) {
            sink_->write(msg);
        } else {
            std::cerr << "Sink not set.\n";
        }
    }

private:
    std::unique_ptr<LogSink> sink_;

    Logger() {
        set_sink(SinkType::CONSOLE);
    }

    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;
};
