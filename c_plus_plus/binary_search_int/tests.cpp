#include "function.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>
#include <iomanip>

std::vector<int> create_sorted_array(int size, int min_val = 0, int max_val = 100) {
    std::vector<int> arr(size);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(min_val, max_val);

    for (int i = 0; i < size; ++i) {
        arr[i] = dis(gen);
    }
    std::sort(arr.begin(), arr.end());
    return arr;
}

void validate_search_result(const std::vector<int>& array, int target, int expected_index) {
    int index = binary_search_int(array, target);
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
    std::vector<std::tuple<std::vector<int>, int, int>> test_cases = {
        {{-2, 1, 1, 4, 7, 9, 10}, 1, 1},
        {{1, 2, 3, 4, 5}, 3, 2},
        {{1, 2, 3, 4, 5}, 6, -1},
        {{1, 2, 2, 4, 4, 4, 5, 6}, 4, 3},
        {{-9, -3, -1, 0, 2, 5}, -3, 1},
        {{1, 1, 1, 2, 2, 3, 3}, 1, 0},
        {{2, 2, 3, 3, 4, 4, 4}, 4, 4}
    };

    double total_execution_time = 0.0;

    // Run the test cases and measure execution time
    for (const auto& test_case : test_cases) {
        const auto& array = std::get<0>(test_case);
        int target = std::get<1>(test_case);
        int expected_index = std::get<2>(test_case);

        try {
            validate_search_result(array, target, expected_index);
            auto start = std::chrono::high_resolution_clock::now();
            for (int i = 0; i < 1000; ++i) {
                binary_search_int(array, target);
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
            std::vector<int> array = create_sorted_array(size);
            int target = array[std::rand() % size];
            int expected_index = std::lower_bound(array.begin(), array.end(), target) - array.begin();
            validate_search_result(array, target, expected_index);
            auto start = std::chrono::high_resolution_clock::now();
            for (int j = 0; j < 1000; ++j) {
                binary_search_int(array, target);
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
    int random_case_count_large = 100;
    for (int i = 0; i < random_case_count_large; ++i) {
        try {
            int size = std::rand() % 9001 + 1000;
            std::vector<int> array = create_sorted_array(size);
            int target = array[std::rand() % size];
            int expected_index = std::lower_bound(array.begin(), array.end(), target) - array.begin();
            validate_search_result(array, target, expected_index);
            auto start = std::chrono::high_resolution_clock::now();
            for (int j = 0; j < 1000; ++j) {
                binary_search_int(array, target);
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
    average_execution_time = average_execution_time / 1000.0;  // each case is run 1000 times
    double average_execution_time_ms = average_execution_time * 1000.0;
    std::cout << std::fixed << std::setprecision(10);
    std::cout << "Average execution time: " << average_execution_time_ms << " ms" << std::endl;

    return 0;
}