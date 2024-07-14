#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include "function.h"

std::vector<double> create_sorted_array(int size, double min_val = 0.0, double max_val = 100.0) {
    std::vector<double> arr(size);
    for (int i = 0; i < size; ++i) {
        arr[i] = min_val + (max_val - min_val) * (static_cast<double>(rand()) / RAND_MAX);
    }
    std::sort(arr.begin(), arr.end());
    return arr;
}

void validate_search_result(const std::vector<double>& array, double target, int expected_index) {
    int index = binary_search_float(array, target);
    if (index != expected_index) {
        std::cout << "Failed test case: array=";
        for (const auto& val : array) {
            std::cout << val << " ";
        }
        std::cout << ", target=" << target << std::endl;
        std::cout << "Expected index: " << expected_index << std::endl;
        std::cout << "Actual index: " << index << std::endl;
        exit(1);
    }
}

int main() {
    srand(static_cast<unsigned>(time(0)));

    std::vector<std::vector<double>> test_arrays;
    std::vector<double> test_targets;
    std::vector<int> test_expected_indices;

    // Manually add test cases
    test_arrays.push_back({-2.5, 1.2, 1.7, 4.0, 7.3, 9.1, 10.8});
    test_targets.push_back(1.7);
    test_expected_indices.push_back(2);

    test_arrays.push_back({1.1, 2.2, 3.3, 4.4, 5.5});
    test_targets.push_back(3.3);
    test_expected_indices.push_back(2);

    // Add more test cases here...

    double total_execution_time = 0.0;

    // Run the test cases and measure execution time
    for (size_t i = 0; i < test_arrays.size(); ++i) {
        try {
            validate_search_result(test_arrays[i], test_targets[i], test_expected_indices[i]);
            clock_t start = clock();
            for (int j = 0; j < 1000; ++j) {
                binary_search_float(test_arrays[i], test_targets[i]);
            }
            clock_t end = clock();
            total_execution_time += static_cast<double>(end - start) / CLOCKS_PER_SEC;
        } catch (const std::exception& e) {
            std::cout << "Error occurred for test case: array=";
            for (const auto& val : test_arrays[i]) {
                std::cout << val << " ";
            }
            std::cout << ", target=" << test_targets[i] << ", error=" << e.what() << std::endl;
            exit(1);
        }
    }

    // Run random test cases with small arrays and measure execution time
    int random_case_count = 1000;
    for (int i = 0; i < random_case_count; ++i) {
        try {
            int size = rand() % 100 + 1;
            std::vector<double> array = create_sorted_array(size);
            double target = array[rand() % size];
            int expected_index = std::lower_bound(array.begin(), array.end(), target) - array.begin();
            validate_search_result(array, target, expected_index);
            clock_t start = clock();
            for (int j = 0; j < 1000; ++j) {
                binary_search_float(array, target);
            }
            clock_t end = clock();
            total_execution_time += static_cast<double>(end - start) / CLOCKS_PER_SEC;
        } catch (const std::exception& e) {
            std::cout << "Error occurred for random small test case" << std::endl;
            exit(1);
        }
    }

    // Run random test cases with large arrays and measure execution time
    int random_case_count_large = 1000;
    for (int i = 0; i < random_case_count_large; ++i) {
        try {
            int size = rand() % 9001 + 1000;
            std::vector<double> array = create_sorted_array(size);
            double target = array[rand() % size];
            int expected_index = std::lower_bound(array.begin(), array.end(), target) - array.begin();
            validate_search_result(array, target, expected_index);
            clock_t start = clock();
            for (int j = 0; j < 1000; ++j) {
                binary_search_float(array, target);
            }
            clock_t end = clock();
            total_execution_time += static_cast<double>(end - start) / CLOCKS_PER_SEC;
        } catch (const std::exception& e) {
            std::cout << "Error occurred for random large test case" << std::endl;
            exit(1);
        }
    }

    double average_execution_time = total_execution_time / (test_arrays.size() + random_case_count + random_case_count_large);
    average_execution_time /= 1000;  // each case is run 1000 times
    double average_execution_time_ms = average_execution_time * 1000;
    std::cout << std::fixed << std::setprecision(10);
    std::cout << "Average execution time: " << average_execution_time_ms << " ms" << std::endl;

    return 0;
}