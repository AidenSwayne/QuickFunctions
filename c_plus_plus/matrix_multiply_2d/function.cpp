#include "function.h"

std::vector<std::vector<int>> matrix_multiply_2d(const std::vector<std::vector<int>>& matrix1, const std::vector<std::vector<int>>& matrix2) {
    if (matrix1[0].size() != matrix2.size()) {
        throw std::invalid_argument("The number of columns in matrix1 must be equal to the number of rows in matrix2.");
    }

    std::vector<std::vector<int>> result(matrix1.size(), std::vector<int>(matrix2[0].size(), 0));

    for (size_t i = 0; i < matrix1.size(); ++i) {
        for (size_t j = 0; j < matrix2[0].size(); ++j) {
            for (size_t k = 0; k < matrix2.size(); ++k) {
                result[i][j] += matrix1[i][k] * matrix2[k][j];
            }
        }
    }

    return result;
}