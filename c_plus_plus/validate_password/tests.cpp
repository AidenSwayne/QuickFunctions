#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <random>
#include <algorithm>
#include "function.h"

void validate_password_wrapper(const std::string& password, bool expected_output) {
    try {
        bool actual_output = validate_password(password);
        if (actual_output != expected_output) {
            std::cout << "Failed test case: password='" << password << "', expected=" << std::boolalpha << expected_output << ", actual=" << actual_output << std::endl;
            exit(1);
        }
    } catch (const std::exception& e) {
        std::cout << "Error occurred for test case: password='" << password << "', error=" << e.what() << std::endl;
        exit(1);
    }
}

std::string generate_random_password(int length) {
    static const char charset[] =
        "abcdefghijklmnopqrstuvwxyz"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "0123456789"
        "!@#$%^&*(),.?\":{}|<>";
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<> dis(0, sizeof(charset) - 2);

    std::string password;
    password.reserve(length);
    for (int i = 0; i < length; ++i) {
        password += charset[dis(gen)];
    }
    return password;
}

int main() {
    std::vector<std::pair<std::string, bool>> test_cases = {
        {"Abcdefg1!", true},
        {"abcdefgh1", false},
        {"ABCDEFGH!", false},
        {"Abcdefgh!", false},
        {"Abcdefg1", false},
        {"Ab1!", false},
        {"abcdefgh", false},
        {"ABCDEFGH", false},
        {"12345678", false},
        {"!@#$%^&*", false},
        {"Abcdefg1!Abcdefg1!Abcdefg1!Abcdefg1!Abcdefg1!Abcdefg1!Abcdefg1!", true},
        {"Abcdefg1!Abcdefg1!Abcdefg1!Abcdefg1!Abcdefg1!Abcdefg1!Abcdefg1!Abcdefg1!", false},
        {"Abcdefg1!Abcdefg1!Abcdefg1!Abcdefg1!Abcdefg1!Abcdefg1!Abcdef", true},
        {"abcdefghijklmnopqrstuvwxyz1234567890!@#$%^&*(),.?\":{}|<>", false},
        {"ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890!@#$%^&*(),.?\":{}|<>", false},
        {"abcdefgh1!", false},
        {"Abcdefgh1", false},
        {"Abcdefgh!", false},
        {"Abc123!@#", true},
        {"Abc123!", false},
        {"Abc123!@#$%^&*(),.?\":{}|<>", true},
        {"abcABC123!@#", true},
        {"abcABC123!", true},
        {"abcABC123!@#$%^&*(),.?\":{}|<>", true},
    };

    double total_execution_time = 0.0;

    // Run the test cases and measure execution time
    for (const auto& test_case : test_cases) {
        validate_password_wrapper(test_case.first, test_case.second);
        auto start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < 100; ++i) {
            validate_password(test_case.first);
        }
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> diff = end - start;
        total_execution_time += diff.count();
    }

    // Run random test cases and measure execution time
    int random_case_count = 1000;
    for (int i = 0; i < random_case_count; ++i) {
        std::string password = generate_random_password(std::rand() % 66 + 5);  // 5 to 70 characters
        bool expected_output = (
            password.length() >= 8 && password.length() <= 64 &&
            std::any_of(password.begin(), password.end(), [](char c) { return std::isupper(c); }) &&
            std::any_of(password.begin(), password.end(), [](char c) { return std::islower(c); }) &&
            std::any_of(password.begin(), password.end(), [](char c) { return std::isdigit(c); }) &&
            std::any_of(password.begin(), password.end(), [](char c) { return std::string("`~!@#$%^&*()-_=+[]{}\|;:'\",./<>?").find(c) != std::string::npos; })
        );
        validate_password_wrapper(password, expected_output);
        auto start = std::chrono::high_resolution_clock::now();
        for (int j = 0; j < 100; ++j) {
            validate_password(password);
        }
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> diff = end - start;
        total_execution_time += diff.count();
    }

    double average_execution_time = total_execution_time / (test_cases.size() + random_case_count);
    average_execution_time /= 100;  // each case is run 100 times
    double average_execution_time_ms = average_execution_time * 1000;
    std::cout << std::fixed << std::setprecision(10);
    std::cout << "Average execution time: " << average_execution_time_ms << " ms" << std::endl;

    return 0;
}