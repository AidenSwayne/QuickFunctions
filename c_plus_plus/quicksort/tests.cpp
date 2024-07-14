#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <chrono>
#include "function.h"

std::vector<int> create_array(int size, int min_val = 0, int max_val = 100) {
    std::vector<int> arr(size);
    for (int i = 0; i < size; ++i) {
        arr[i] = rand() % (max_val - min_val + 1) + min_val;
    }
    return arr;
}

void validate_sorted_array(const std::vector<int>& array, const std::vector<int>& expected_result) {
    if (array != expected_result) {
        std::cout << "Failed test case: array=";
        for (const auto& val : array) {
            std::cout << val << " ";
        }
        std::cout << std::endl;
        std::cout << "Expected output: ";
        for (const auto& val : expected_result) {
            std::cout << val << " ";
        }
        std::cout << std::endl;
        std::cout << "Actual output: ";
        for (const auto& val : array) {
            std::cout << val << " ";
        }
        std::cout << std::endl;
        exit(1);
    }
}

int main() {
    srand(static_cast<unsigned>(time(0)));

    std::vector<std::pair<std::vector<int>, std::vector<int>>> test_cases = {
        {{1, 7, 4, 1, 10, 9, -2}, {-2, 1, 1, 4, 7, 9, 10}},
        {{1, 2, 3, 4, 5}, {1, 2, 3, 4, 5}},
        {{5, 4, 3, 2, 1}, {1, 2, 3, 4, 5}},
        {{4, 2, 6, 4, 1, 4, 2, 5}, {1, 2, 2, 4, 4, 4, 5, 6}},
        {{-3, 0, -9, 5, -1, 2}, {-9, -3, -1, 0, 2, 5}}
    };

    double total_execution_time = 0.0;

    // Run the test cases and measure execution time
    for (const auto& test_case : test_cases) {
        try {
            auto array = test_case.first;
            std::vector<int> sorted_array = quicksort(array);
            validate_sorted_array(sorted_array, test_case.second);
            auto start = std::chrono::high_resolution_clock::now();
            for (int i = 0; i < 100; ++i) {
                quicksort(array);
            }
            auto end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> diff = end - start;
            total_execution_time += diff.count();
        } catch (const std::exception& e) {
            std::cout << "Error occurred for test case: array=";
            for (const auto& val : test_case.first) {
                std::cout << val << " ";
            }
            std::cout << ", error=" << e.what() << std::endl;
            exit(1);
        }
    }

    // Run random test cases with small arrays and measure execution time
    int random_case_count = 100;
    for (int i = 0; i < random_case_count; ++i) {
        try {
            int size = rand() % 100 + 1;
            std::vector<int> array = create_array(size);
            std::vector<int> expected_result = array;
            std::sort(expected_result.begin(), expected_result.end());
            std::vector<int> sorted_array = quicksort(array);
            validate_sorted_array(sorted_array, expected_result);
            auto start = std::chrono::high_resolution_clock::now();
            for (int j = 0; j < 100; ++j) {
                quicksort(array);
            }
            auto end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> diff = end - start;
            total_execution_time += diff.count();
        } catch (const std::exception& e) {
            std::cout << "Error occurred for random small test case" << std::endl;
            exit(1);
        }
    }

    // Run random test cases with large arrays and measure execution time
    int random_case_count_large = 100;
    for (int i = 0; i < random_case_count_large; ++i) {
        try {
            int size = rand() % 9001 + 1000;
            std::vector<int> array = create_array(size);
            std::vector<int> expected_result = array;
            std::sort(expected_result.begin(), expected_result.end());
            std::vector<int> sorted_array = quicksort(array);
            validate_sorted_array(sorted_array, expected_result);
            auto start = std::chrono::high_resolution_clock::now();
            for (int j = 0; j < 100; ++j) {
                quicksort(array);
            }
            auto end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> diff = end - start;
            total_execution_time += diff.count();
        } catch (const std::exception& e) {
            std::cout << "Error occurred for random large test case" << std::endl;
            exit(1);
        }
    }

    double average_execution_time = total_execution_time / (test_cases.size() + random_case_count + random_case_count_large);
    average_execution_time /= 100;  // each case is run 100 times
    double average_execution_time_ms = average_execution_time * 1000;
    std::cout << std::fixed << std::setprecision(10);
    std::cout << "Average execution time: " << average_execution_time_ms << " ms" << std::endl;

    return 0;
}