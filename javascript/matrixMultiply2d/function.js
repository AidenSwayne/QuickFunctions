function matrixMultiply2d(matrix1, matrix2) {
    if (matrix1[0].length !== matrix2.length) {
        throw new Error("The number of columns in matrix1 must be equal to the number of rows in matrix2.");
    }

    const result = Array(matrix1.length).fill().map(() => Array(matrix2[0].length).fill(0));
    for (let i = 0; i < matrix1.length; i++) {
        for (let j = 0; j < matrix2[0].length; j++) {
            for (let k = 0; k < matrix2.length; k++) {
                result[i][j] += matrix1[i][k] * matrix2[k][j];
            }
        }
    }

    return result;
}
