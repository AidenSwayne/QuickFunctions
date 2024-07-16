#include "function.h"

std::vector<std::vector<int>> matrix_multiply_2d(const std::vector<std::vector<int>>& matrix1, const std::vector<std::vector<int>>& matrix2) {
    if (matrix1[0].size() != matrix2.size()) {
        throw std::invalid_argument("The number of columns in matrix1 must be equal to the number of rows in matrix2.");
    }

    size_t n = matrix1.size();
    size_t m = matrix2[0].size();
    size_t p = matrix2.size();

    std::vector<std::vector<int>> result(n, std::vector<int>(m, 0));

    for (size_t i = 0; i < n; ++i) {
        for (size_t k = 0; k < p; ++k) {
            int temp = matrix1[i][k];
            for (size_t j = 0; j < m; ++j) {
                result[i][j] += temp * matrix2[k][j];
            }
        }
    }

    return result;
}
