from function import merge_sort
import timeit
import random

def create_array(size: int, min_val: int = 0, max_val: int = 100) -> list[int]:
    return [random.randint(min_val, max_val) for _ in range(size)]

def validate_sorted_array(array: list[int], expected_result: list[int]) -> None:
    if array != expected_result:
        print(f"Failed test case: array={array}")
        print(f"Expected output: {expected_result}")
        print(f"Actual output: {array}")
        exit(1)

test_cases = [
    ([1, 7, 4, 1, 10, 9, -2], [-2, 1, 1, 4, 7, 9, 10]),
    ([1, 2, 3, 4, 5], [1, 2, 3, 4, 5]),
    ([5, 4, 3, 2, 1], [1, 2, 3, 4, 5]),
    ([4, 2, 6, 4, 1, 4, 2, 5], [1, 2, 2, 4, 4, 4, 5, 6]),
    ([-3, 0, -9, 5, -1, 2], [-9, -3, -1, 0, 2, 5])
]

total_execution_time = 0

# Run the test cases and measure execution time
for array, expected_result in test_cases:
    try:
        sorted_array = merge_sort(array)
        validate_sorted_array(sorted_array, expected_result)
        execution_time = timeit.timeit(lambda: merge_sort(array), number=1000)
        total_execution_time += execution_time
    except Exception as e:
        print(f"Error occurred for test case: array={array}, error={str(e)}")
        exit(1)

# Run random test cases with small arrays and measure execution time
random_case_count = 100
for _ in range(random_case_count):
    try:
        size = random.randint(1, 100)
        array = create_array(size)
        expected_result = sorted(array)
        sorted_array = merge_sort(array)
        validate_sorted_array(sorted_array, expected_result)
        execution_time = timeit.timeit(lambda: merge_sort(array), number=1000)
        total_execution_time += execution_time
    except Exception as e:
        print(f"Error occurred for test case: array={array}, error={str(e)}")
        exit(1)

# Run random test cases with large arrays and measure execution time
random_case_count_large = 10
for _ in range(random_case_count_large):
    try:
        size = random.randint(1000, 10000)
        array = create_array(size)
        expected_result = sorted(array)
        sorted_array = merge_sort(array)
        validate_sorted_array(sorted_array, expected_result)
        execution_time = timeit.timeit(lambda: merge_sort(array), number=1000)
        total_execution_time += execution_time
    except Exception as e:
        print(f"Error occurred for test case: array={array}, error={str(e)}")
        exit(1)

average_execution_time = total_execution_time / (len(test_cases) + random_case_count + random_case_count_large)
average_execution_time = average_execution_time / 1000  # each case is run 1000 times
average_execution_time_ms = average_execution_time * 1000
print(f"Average execution time: {average_execution_time_ms:.10f} ms")