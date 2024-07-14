#include "function.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>
#include <iomanip>

std::vector<std::string> create_sorted_array(int size, int min_length = 1, int max_length = 10) {
    std::vector<std::string> arr;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> len_dis(min_length, max_length);
    std::uniform_int_distribution<> char_dis(0, 25);

    for (int i = 0; i < size; ++i) {
        int length = len_dis(gen);
        std::string s;
        for (int j = 0; j < length; ++j) {
            s += static_cast<char>('a' + char_dis(gen));
        }
        arr.push_back(s);
    }
    std::sort(arr.begin(), arr.end());
    return arr;
}

void validate_search_result(const std::vector<std::string>& array, const std::string& target, int expected_index) {
    int index = binary_search_string(array, target);
    if (index != expected_index) {
        std::cout << "Failed test case: array=[";
        for (size_t i = 0; i < array.size(); ++i) {
            std::cout << array[i];
            if (i < array.size() - 1) std::cout << ", ";
        }
        std::cout << "], target=" << target << std::endl;
        std::cout << "Expected index: " << expected_index << std::endl;
        std::cout << "Actual index: " << index << std::endl;
        exit(1);
    }
}

int main() {
    std::vector<std::tuple<std::vector<std::string>, std::string, int>> test_cases = {
        {{"apple", "banana", "cherry", "date", "elderberry"}, "cherry", 2},
        {{"cat", "dog", "elephant", "fox", "giraffe"}, "horse", -1},
        {{"alice", "bob", "charlie", "david", "eve"}, "alice", 0},
        {{"alpha", "beta", "beta", "gamma", "gamma", "gamma", "delta"}, "gamma", 3},
        {{"blue", "green", "indigo", "orange", "red", "yellow"}, "orange", 3},
        {{"a", "a", "a", "b", "b", "c", "c"}, "a", 0},
        {{"x", "x", "y", "y", "z", "z", "z"}, "z", 4}
    };

    double total_execution_time = 0.0;

    // Run the test cases and measure execution time
    for (const auto& test_case : test_cases) {
        const auto& array = std::get<0>(test_case);
        const auto& target = std::get<1>(test_case);
        int expected_index = std::get<2>(test_case);

        try {
            validate_search_result(array, target, expected_index);
            auto start = std::chrono::high_resolution_clock::now();
            for (int i = 0; i < 100; ++i) {
                binary_search_string(array, target);
            }
            auto end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> diff = end - start;
            total_execution_time += diff.count();
        } catch (const std::exception& e) {
            std::cout << "Error occurred for test case: array=[";
            for (size_t i = 0; i < array.size(); ++i) {
                std::cout << array[i];
                if (i < array.size() - 1) std::cout << ", ";
            }
            std::cout << "], target=" << target << ", error=" << e.what() << std::endl;
            return 1;
        }
    }

    // Run random test cases with small arrays and measure execution time
    int random_case_count = 100;
    for (int i = 0; i < random_case_count; ++i) {
        try {
            int size = std::rand() % 100 + 1;
            std::vector<std::string> array = create_sorted_array(size);
            std::string target = array[std::rand() % size];
            int expected_index = std::find(array.begin(), array.end(), target) - array.begin();
            validate_search_result(array, target, expected_index);
            auto start = std::chrono::high_resolution_clock::now();
            for (int j = 0; j < 100; ++j) {
                binary_search_string(array, target);
            }
            auto end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> diff = end - start;
            total_execution_time += diff.count();
        } catch (const std::exception& e) {
            std::cout << "Error occurred for random test case: " << e.what() << std::endl;
            return 1;
        }
    }

    // Run random test cases with large arrays and measure execution time
    int random_case_count_large = 10;
    for (int i = 0; i < random_case_count_large; ++i) {
        try {
            int size = std::rand() % 9001 + 1000;
            std::vector<std::string> array = create_sorted_array(size);
            std::string target = array[std::rand() % size];
            int expected_index = std::find(array.begin(), array.end(), target) - array.begin();
            validate_search_result(array, target, expected_index);
            auto start = std::chrono::high_resolution_clock::now();
            for (int j = 0; j < 100; ++j) {
                binary_search_string(array, target);
            }
            auto end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> diff = end - start;
            total_execution_time += diff.count();
        } catch (const std::exception& e) {
            std::cout << "Error occurred for random large test case: " << e.what() << std::endl;
            return 1;
        }
    }

    double average_execution_time = total_execution_time / (test_cases.size() + random_case_count + random_case_count_large);
    average_execution_time = average_execution_time / 100.0;  // each case is run 100 times
    double average_execution_time_ms = average_execution_time * 1000.0;
    std::cout << std::fixed << std::setprecision(10);
    std::cout << "Average execution time: " << average_execution_time_ms << " ms" << std::endl;

    return 0;
}