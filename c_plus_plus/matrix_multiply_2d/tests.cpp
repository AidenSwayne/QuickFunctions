#include "function.h"
#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <iomanip>

std::vector<std::vector<int>> create_matrix(int rows, int cols, int min_val = 0, int max_val = 10) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(min_val, max_val);

    std::vector<std::vector<int>> matrix(rows, std::vector<int>(cols));
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            matrix[i][j] = dis(gen);
        }
    }
    return matrix;
}

void validate_matrix_multiplication(const std::vector<std::vector<int>>& matrix1, 
                                    const std::vector<std::vector<int>>& matrix2, 
                                    const std::vector<std::vector<int>>& expected_result) {
    try {
        auto result = matrix_multiply_2d(matrix1, matrix2);
        if (result != expected_result) {
            std::cout << "Failed test case: matrix1=" << std::endl;
            for (const auto& row : matrix1) {
                for (int val : row) std::cout << val << " ";
                std::cout << std::endl;
            }
            std::cout << "matrix2=" << std::endl;
            for (const auto& row : matrix2) {
                for (int val : row) std::cout << val << " ";
                std::cout << std::endl;
            }
            exit(1);
        }
    } catch (const std::invalid_argument& e) {
        if (std::string(e.what()) != "The number of columns in matrix1 must be equal to the number of rows in matrix2.") {
            std::cout << "Error occurred for test case: " << e.what() << std::endl;
            exit(1);
        }
    }
}

int main() {
    std::vector<std::tuple<std::vector<std::vector<int>>, std::vector<std::vector<int>>, std::vector<std::vector<int>>>> test_cases = {
        {{{1, 2}, {3, 4}}, {{5, 6}, {7, 8}}, {{19, 22}, {43, 50}}},
        {{{1, 2, 3}}, {{4}, {5}, {6}}, {{32}}},
        {{{1, 2}, {3, 4}, {5, 6}}, {{7, 8, 9}, {10, 11, 12}}, {{27, 30, 33}, {61, 68, 75}, {95, 106, 117}}}
    };

    double total_execution_time = 0.0;

    // Run the test cases and measure execution time
    for (const auto& [matrix1, matrix2, expected_result] : test_cases) {
        validate_matrix_multiplication(matrix1, matrix2, expected_result);
        auto start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < 100; ++i) {
            matrix_multiply_2d(matrix1, matrix2);
        }
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> diff = end - start;
        total_execution_time += diff.count();
    }

    // Run random test cases with small matrices and measure execution time
    int random_case_count = 100;
    for (int i = 0; i < random_case_count; ++i) {
        int rows1 = std::rand() % 10 + 1, cols1 = std::rand() % 10 + 1;
        int rows2 = cols1, cols2 = std::rand() % 10 + 1;
        auto matrix1 = create_matrix(rows1, cols1);
        auto matrix2 = create_matrix(rows2, cols2);

        std::vector<std::vector<int>> expected_result(rows1, std::vector<int>(cols2, 0));
        for (int i = 0; i < rows1; ++i) {
            for (int j = 0; j < cols2; ++j) {
                for (int k = 0; k < cols1; ++k) {
                    expected_result[i][j] += matrix1[i][k] * matrix2[k][j];
                }
            }
        }

        validate_matrix_multiplication(matrix1, matrix2, expected_result);

        auto start = std::chrono::high_resolution_clock::now();
        for (int j = 0; j < 100; ++j) {
            matrix_multiply_2d(matrix1, matrix2);
        }
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> diff = end - start;
        total_execution_time += diff.count();
    }

    // Run random test cases with large matrices and measure execution time
    int random_case_count_large = 10;
    for (int i = 0; i < random_case_count_large; ++i) {
        int rows1 = std::rand() % 41 + 10, cols1 = std::rand() % 41 + 10;
        int rows2 = cols1, cols2 = std::rand() % 41 + 10;
        auto matrix1 = create_matrix(rows1, cols1);
        auto matrix2 = create_matrix(rows2, cols2);

        std::vector<std::vector<int>> expected_result(rows1, std::vector<int>(cols2, 0));
        for (int i = 0; i < rows1; ++i) {
            for (int j = 0; j < cols2; ++j) {
                for (int k = 0; k < cols1; ++k) {
                    expected_result[i][j] += matrix1[i][k] * matrix2[k][j];
                }
            }
        }

        validate_matrix_multiplication(matrix1, matrix2, expected_result);

        auto start = std::chrono::high_resolution_clock::now();
        for (int j = 0; j < 100; ++j) {
            matrix_multiply_2d(matrix1, matrix2);
        }
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> diff = end - start;
        total_execution_time += diff.count();
    }

    double average_execution_time = total_execution_time / (test_cases.size() + random_case_count + random_case_count_large);
    average_execution_time = average_execution_time / 100.0;  // each case is run 100 times
    double average_execution_time_ms = average_execution_time * 1000.0;
    std::cout << std::fixed << std::setprecision(10);
    std::cout << "Average execution time: " << average_execution_time_ms << " ms" << std::endl;

    return 0;
}