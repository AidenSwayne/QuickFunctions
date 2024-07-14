#include "function.h"
#include <iostream>
#include <chrono>
#include <vector>
#include <random>
#include <set>
#include <complex>
#include <sstream>
#include <iomanip>
#include <functional>
#include <map>

std::string custom_json_encode(const std::any& data);

void validate_json_encode(const std::any& input_data, const std::string& expected_output) {
    try {
        std::string actual_output = json_encode(input_data);
        if (actual_output != expected_output) {
            std::cout << "Failed test case: input='" << custom_json_encode(input_data) << "', expected=" << expected_output << ", actual=" << actual_output << std::endl;
            exit(1);
        }
    } catch (const std::exception& e) {
        std::cout << "Error occurred for test case: input='" << custom_json_encode(input_data) << "', error=" << e.what() << std::endl;
        exit(1);
    }
}

std::string custom_json_encode(const std::any& data) {
    std::ostringstream oss;
    
    std::function<void(const std::any&)> encode = [&](const std::any& obj) {
        if (obj.type() == typeid(std::map<std::string, std::any>)) {
            const auto& map = std::any_cast<const std::map<std::string, std::any>&>(obj);
            oss << '{';
            bool first = true;
            for (const auto& [key, value] : map) {
                if (!first) oss << ',';
                oss << '"' << key << "\":";
                encode(value);
                first = false;
            }
            oss << '}';
        } else if (obj.type() == typeid(std::vector<std::any>)) {
            const auto& vec = std::any_cast<const std::vector<std::any>&>(obj);
            oss << '[';
            bool first = true;
            for (const auto& item : vec) {
                if (!first) oss << ',';
                encode(item);
                first = false;
            }
            oss << ']';
        } else if (obj.type() == typeid(std::set<int>)) {
            const auto& set = std::any_cast<const std::set<int>&>(obj);
            oss << '[';
            bool first = true;
            for (const auto& item : set) {
                if (!first) oss << ',';
                oss << item;
                first = false;
            }
            oss << ']';
        } else if (obj.type() == typeid(std::complex<double>)) {
            auto c = std::any_cast<std::complex<double>>(obj);
            oss << '[' << c.real() << ',' << c.imag() << ']';
        } else if (obj.type() == typeid(double)) {
            double d = std::any_cast<double>(obj);
            if (std::isinf(d)) {
                oss << (d > 0 ? "\"Infinity\"" : "\"-Infinity\"");
            } else if (std::isnan(d)) {
                oss << "\"NaN\"";
            } else {
                oss << std::setprecision(17) << d;
            }
        } else if (obj.type() == typeid(int)) {
            oss << std::any_cast<int>(obj);
        } else if (obj.type() == typeid(std::string)) {
            const auto& s = std::any_cast<const std::string&>(obj);
            oss << '"';
            for (char c : s) {
                switch (c) {
                    case '"': oss << "\\\""; break;
                    case '\\': oss << "\\\\"; break;
                    case '\b': oss << "\\b"; break;
                    case '\f': oss << "\\f"; break;
                    case '\n': oss << "\\n"; break;
                    case '\r': oss << "\\r"; break;
                    case '\t': oss << "\\t"; break;
                    default:
                        if ('\x00' <= c && c <= '\x1f') {
                            oss << "\\u" << std::hex << std::setw(4) << std::setfill('0') << static_cast<int>(c);
                        } else {
                            oss << c;
                        }
                }
            }
            oss << '"';
        } else {
            oss << "null";
        }
    };
    
    encode(data);
    return oss.str();
}

int main() {
    std::vector<std::pair<std::any, std::string>> test_cases = {
        {std::map<std::string, std::any>{{"name", "John"}, {"age", 30}}, R"({"name":"John","age":30})"},
        {std::vector<std::any>{1, 2, 3}, "[1,2,3]"},
        {std::map<std::string, std::any>{{"set", std::set<int>{1, 2, 3}}}, R"({"set":[1,2,3]})"},
        {std::map<std::string, std::any>{{"complex", std::complex<double>(1, 2)}}, R"({"complex":[1,2]})"},
        {std::map<std::string, std::any>{{"invalid", std::numeric_limits<double>::infinity()}}, R"({"invalid":"Infinity"})"},
        {std::map<std::string, std::any>{{"invalid", -std::numeric_limits<double>::infinity()}}, R"({"invalid":"-Infinity"})"},
        {std::map<std::string, std::any>{{"invalid", std::numeric_limits<double>::quiet_NaN()}}, R"({"invalid":"NaN"})"},
    };

    double total_execution_time = 0.0;
    // Run the test cases and measure execution time
    for (const auto& [input_data, expected_output] : test_cases) {
        validate_json_encode(input_data, expected_output);
        auto start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < 1000; ++i) {
            json_encode(input_data);
        }
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> diff = end - start;
        total_execution_time += diff.count();
    }

    // Run random test cases and measure execution time
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 100);
    int random_case_count = 100;
    for (int i = 0; i < random_case_count; ++i) {
        std::any input_data;
        switch (dis(gen) % 4) {
            case 0:
                input_data = std::map<std::string, std::any>{
                    {"key", "value"}, 
                    {"nested", std::map<std::string, std::any>{{"key", "value"}}}
                };
                break;
            case 1: {
                std::vector<std::any> arr;
                for (int j = 0; j < dis(gen) % 10 + 1; ++j) {
                    arr.push_back(dis(gen));
                }
                input_data = arr;
                break;
            }
            case 2: {
                std::set<int> s;
                for (int j = 0; j < dis(gen) % 10 + 1; ++j) {
                    s.insert(dis(gen));
                }
                input_data = std::map<std::string, std::any>{{"set", s}};
                break;
            }
            case 3:
                input_data = std::map<std::string, std::any>{{"complex", std::complex<double>(dis(gen), dis(gen))}};
                break;
        }
        std::string expected_output = custom_json_encode(input_data);
        validate_json_encode(input_data, expected_output);
        auto start = std::chrono::high_resolution_clock::now();
        for (int j = 0; j < 1000; ++j) {
            json_encode(input_data);
        }
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> diff = end - start;
        total_execution_time += diff.count();
    }

    double average_execution_time = total_execution_time / (test_cases.size() + random_case_count);
    average_execution_time = average_execution_time / 1000.0;  // each case is run 1000 times
    double average_execution_time_ms = average_execution_time * 1000.0;
    std::cout << std::fixed << std::setprecision(10);
    std::cout << "Average execution time: " << average_execution_time_ms << " ms" << std::endl;
    return 0;
}
