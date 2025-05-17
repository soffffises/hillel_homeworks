#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <memory>


class INumberReader {
public:
    virtual std::vector<int> read(const std::string& filename) = 0;
    virtual ~INumberReader() = default;
};

class INumberFilter {
public:
    virtual bool keep(int number) = 0;
    virtual ~INumberFilter() = default;
};

class INumberObserver {
public:
    virtual void on_number(int number) = 0;
    virtual void on_finished() = 0;
    virtual ~INumberObserver() = default;
};


class FileNumberReader : public INumberReader {
public:
    std::vector<int> read(const std::string& filename) override {
        std::ifstream file(filename);
        std::vector<int> numbers;

        if (!file.is_open()) {
            std::cerr << " Error: could not open file \"" << filename << "\"\n";
            return {};
        }

        int num;
        while (file >> num) {
            numbers.push_back(num);
        }

        if (numbers.empty()) {
            std::cerr << " Warning: file contains no valid integers.\n";
        }

        return numbers;
    }
};

class EvenFilter : public INumberFilter {
public:
    bool keep(int number) override {
        return number % 2 == 0;
    }
};

class OddFilter : public INumberFilter {
public:
    bool keep(int number) override {
        return number % 2 != 0;
    }
};

class GreaterThanFilter : public INumberFilter {
    int threshold;
public:
    GreaterThanFilter(int n) : threshold(n) {}
    bool keep(int number) override {
        return number > threshold;
    }
};

class FilterFactory {
public:
    static std::unique_ptr<INumberFilter> create(const std::string& filterStr) {
        if (filterStr == "EVEN") {
            return std::make_unique<EvenFilter>();
        } else if (filterStr == "ODD") {
            return std::make_unique<OddFilter>();
        } else if (filterStr.rfind("GT", 0) == 0) {
            try {
                std::string value = filterStr.substr(2);
                if (value.empty()) throw std::invalid_argument("empty");
                int n = std::stoi(value);
                return std::make_unique<GreaterThanFilter>(n);
            } catch (...) {
                std::cerr << " Invalid GT format. Use GT<n>, e.g. GT5\n";
                return nullptr;
            }
        } else {
            std::cerr << " Unknown filter: " << filterStr << "\n";
            return nullptr;
        }
    }
};

class PrintObserver : public INumberObserver {
public:
    void on_number(int number) override {
        std::cout << number << "\n";
    }

    void on_finished() override {}
};

class CountObserver : public INumberObserver {
    int count = 0;
public:
    void on_number(int) override {
        ++count;
    }

    void on_finished() override {
        std::cout << " Total numbers passed the filter: " << count << "\n";
        if (count == 0) {
            std::cout << " No numbers passed the filter.\n";
        }
    }
};

class NumberProcessor {
    INumberReader& reader;
    INumberFilter& filter;
    std::vector<INumberObserver*> observers;

public:
    NumberProcessor(INumberReader& r, INumberFilter& f, const std::vector<INumberObserver*>& obs)
        : reader(r), filter(f), observers(obs) {}

    void run(const std::string& filename) {
        auto numbers = reader.read(filename);
        if (numbers.empty()) {
            std::cerr << " Terminating: no numbers to process.\n";
            return;
        }

        for (int num : numbers) {
            if (filter.keep(num)) {
                for (auto obs : observers) {
                    obs->on_number(num);
                }
            }
        }

        for (auto obs : observers) {
            obs->on_finished();
        }
    }
};


int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << " Usage: <FileName> <FILTER> <Text FILENAME>\n";
        std::cerr << " Available filters:\n";
        std::cerr << " EVEN - even numbers only\n";
        std::cerr << " ODD - odd numbers only\n";
        std::cerr << " GT<n> - numbers greater than n (e.g., GT5)\n";
        return 1;
    }

    std::string filterArg = argv[1];
    std::string filename = argv[2];

    auto filter = FilterFactory::create(filterArg);
    if (!filter) return 1;

    FileNumberReader reader;
    PrintObserver printer;
    CountObserver counter;
    std::vector<INumberObserver*> observers = { &printer, &counter };

    NumberProcessor processor(reader, *filter, observers);
    processor.run(filename);

    return 0;
}
