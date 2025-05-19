#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <memory>
#include <map>
#include <functional>
#include <string>
#include <algorithm>
#include <cctype>


struct INumberReader {
    virtual ~INumberReader() = default;
    virtual std::vector<int> read_numbers(const std::string& filename) = 0;
};

struct INumberFilter {
    virtual ~INumberFilter() = default;
    virtual bool keep(int n) = 0;
};

struct INumberObserver {
    virtual ~INumberObserver() = default;
    virtual void on_number(int n) = 0;
    virtual void on_finished() = 0;
};


class FileNumberReader : public INumberReader {
public:
    std::vector<int> read_numbers(const std::string& filename) override {
        std::ifstream file(filename);
        std::vector<int> numbers;
        if (!file.is_open()) {
            std::cerr << "Failed to open file: " << filename << std::endl;
            return numbers;
        }

        int number;
        while (file >> number) {
            numbers.push_back(number);
        }
        return numbers;
    }
};


class EvenFilter : public INumberFilter {
public:
    bool keep(int n) override { return n % 2 == 0; }
};

class OddFilter : public INumberFilter {
public:
    bool keep(int n) override { return n % 2 != 0; }
};

class GreaterThanFilter : public INumberFilter {
    int threshold;
public:
    GreaterThanFilter(int t) : threshold(t) {}
    bool keep(int n) override { return n > threshold; }
};


class PrintObserver : public INumberObserver {
public:
    void on_number(int n) override {
        std::cout << "Number: " << n << std::endl;
    }
    void on_finished() override {}
};

class CountObserver : public INumberObserver {
    int count = 0;
public:
    void on_number(int) override {
        count++;
    }
    void on_finished() override {
        std::cout << "Total numbers passed filter: " << count << std::endl;
    }
};


class FilterFactory {
    using FilterCreator = std::function<std::unique_ptr<INumberFilter>(const std::string&)>;
    std::map<std::string, FilterCreator> registry;

    FilterFactory() {
        register_filter("EVEN", [](const std::string&) {
            return std::make_unique<EvenFilter>();
        });

        register_filter("ODD", [](const std::string&) {
            return std::make_unique<OddFilter>();
        });

        register_filter("GT", [](const std::string& param) {
            if (param.empty()) {
                throw std::invalid_argument("Missing parameter for GT filter");
            }
            try {
                int val = std::stoi(param);
                return std::make_unique<GreaterThanFilter>(val);
            } catch (...) {
                throw std::invalid_argument("Invalid parameter for GT filter: " + param);
            }
        });
    }

    void register_filter(const std::string& name, FilterCreator creator) {
        registry[name] = creator;
    }

public:
    static FilterFactory& instance() {
        static FilterFactory instance;
        return instance;
    }

    std::unique_ptr<INumberFilter> create(const std::string& filter_expr) {
        std::string key;
        std::string param;

        size_t i = 0;
        while (i < filter_expr.size() && std::isalpha(static_cast<unsigned char>(filter_expr[i]))) {
            key += filter_expr[i++];
        }
        param = filter_expr.substr(i);

        auto it = registry.find(key);
        if (it != registry.end()) {
            return it->second(param);
        }

        throw std::invalid_argument("Unknown filter: " + key);
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
        auto numbers = reader.read_numbers(filename);
        for (int n : numbers) {
            if (filter.keep(n)) {
                for (auto obs : observers) {
                    obs->on_number(n);
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
        std::cerr << "Usage: " << argv[0] << " <FILTER> <FILENAME>\n";
        std::cerr << "FILTER options: EVEN, ODD, GT<n> (e.g., GT5)\n";
        return 1;
    }

    std::string filter_expr = argv[1];
    std::string filename = argv[2];

    try {
        auto& factory = FilterFactory::instance();
        auto filter = factory.create(filter_expr);

        FileNumberReader reader;
        PrintObserver printObs;
        CountObserver countObs;

        std::vector<INumberObserver*> observers = { &printObs, &countObs };

        NumberProcessor processor(reader, *filter, observers);
        processor.run(filename);

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 2;
    }

    return 0;
}
