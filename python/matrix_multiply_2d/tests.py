from function import matrix_multiply_2d
import timeit
import random

def create_matrix(rows, cols, min_val=0, max_val=10):
    return [[random.randint(min_val, max_val) for _ in range(cols)] for _ in range(rows)]

def validate_matrix_multiplication(matrix1, matrix2, expected_result):
    try:
        result = matrix_multiply_2d(matrix1, matrix2)
        if not isinstance(result, list) or not all(isinstance(row, list) for row in result):
            print(f"Failed test case: matrix1={matrix1}, matrix2={matrix2}")
            print(f"Expected return type: List[List[int]], Actual return type: {type(result)}")
            exit(1)
        if result != expected_result:
            print(f"Failed test case: matrix1={matrix1}, matrix2={matrix2}")
            exit(1)
    except ValueError as e:
        if str(e) != "The number of columns in matrix1 must be equal to the number of rows in matrix2.":
            print(f"Error occurred for test case: matrix1={matrix1}, matrix2={matrix2}, error={str(e)}")
            exit(1)

test_cases = [
    ([[1, 2], [3, 4]], [[5, 6], [7, 8]], [[19, 22], [43, 50]]),
    ([[1, 2, 3]], [[4], [5], [6]], [[32]]),
    ([[1, 2], [3, 4], [5, 6]], [[7, 8, 9], [10, 11, 12]], [[27, 30, 33], [61, 68, 75], [95, 106, 117]])
]

total_execution_time = 0

# Run the test cases and measure execution time
for matrix1, matrix2, expected_result in test_cases:
    if expected_result is None:
        validate_matrix_multiplication(matrix1, matrix2, expected_result)
    else:
        try:
            validate_matrix_multiplication(matrix1, matrix2, expected_result)
            execution_time = timeit.timeit(lambda: matrix_multiply_2d(matrix1, matrix2), number=100)
            total_execution_time += execution_time
        except Exception as e:
            print(f"Error occurred for test case: matrix1={matrix1}, matrix2={matrix2}, error={str(e)}")
            exit(1)

# Run random test cases with small matrices and measure execution time
random_case_count = 100
for _ in range(random_case_count):
    try:
        rows1, cols1 = random.randint(1, 10), random.randint(1, 10)
        rows2, cols2 = cols1, random.randint(1, 10)
        matrix1 = create_matrix(rows1, cols1)
        matrix2 = create_matrix(rows2, cols2)

        expected_result = [[sum(a*b for a,b in zip(row,col)) for col in zip(*matrix2)] for row in matrix1]
        validate_matrix_multiplication(matrix1, matrix2, expected_result)

        execution_time = timeit.timeit(lambda: matrix_multiply_2d(matrix1, matrix2), number=100)
        total_execution_time += execution_time
    except Exception as e:
        print(f"Error occurred for test case: matrix1={matrix1}, matrix2={matrix2}, error={str(e)}")
        exit(1)

# Run random test cases with large matrices and measure execution time
random_case_count_large = 10
for _ in range(random_case_count_large):
    try:
        rows1, cols1 = random.randint(10, 50), random.randint(10, 50)
        rows2, cols2 = cols1, random.randint(10, 50)
        matrix1 = create_matrix(rows1, cols1)
        matrix2 = create_matrix(rows2, cols2)

        expected_result = [[sum(a*b for a,b in zip(row,col)) for col in zip(*matrix2)] for row in matrix1]
        validate_matrix_multiplication(matrix1, matrix2, expected_result)

        execution_time = timeit.timeit(lambda: matrix_multiply_2d(matrix1, matrix2), number=100)
        total_execution_time += execution_time
    except Exception as e:
        print(f"Error occurred for test case: matrix1={matrix1}, matrix2={matrix2}, error={str(e)}")
        exit(1)

average_execution_time = total_execution_time / (len(test_cases) + random_case_count + random_case_count_large)
average_execution_time = average_execution_time / 100  # each case is run 100 times
average_execution_time_ms = average_execution_time * 1000
print(f"Average execution time: {average_execution_time_ms:.10f} ms")