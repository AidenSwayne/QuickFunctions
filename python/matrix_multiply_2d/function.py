def matrix_multiply_2d(matrix1: list[list[int]], matrix2: list[list[int]]) -> list[list[int]]:
    if len(matrix1[0]) != len(matrix2):
        raise ValueError("The number of columns in matrix1 must be equal to the number of rows in matrix2.")

    
    result = [[0 for _ in range(len(matrix2[0]))] for _ in range(len(matrix1))]

    for i in range(len(matrix1)):
        for j in range(len(matrix2[0])):
            for k in range(len(matrix2)):
                result[i][j] += matrix1[i][k] * matrix2[k][j]

    return result

