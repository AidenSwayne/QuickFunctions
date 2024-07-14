from function import binary_search_float
import timeit
import random

def create_sorted_array(size: int, min_val: float = 0.0, max_val: float = 100.0) -> list[float]:
    return sorted([random.uniform(min_val, max_val) for _ in range(size)])

def validate_search_result(array: list[float], target: float, expected_index: int) -> None:
    index = binary_search_float(array, target)
    if index != expected_index:
        print(f"Failed test case: array={array}, target={target}")
        print(f"Expected index: {expected_index}")
        print(f"Actual index: {index}")
        exit(1)

test_cases = [
    ([-2.5, 1.2, 1.7, 4.0, 7.3, 9.1, 10.8], 1.7, 2),
    ([1.1, 2.2, 3.3, 4.4, 5.5], 3.3, 2),
    ([1.1, 2.2, 3.3, 4.4, 5.5], 6.6, -1),
    ([1.1, 2.2, 2.2, 4.4, 4.4, 4.4, 5.5, 6.6], 4.4, 3),
    ([-9.9, -3.3, -1.1, 0.0, 2.2, 5.5], -3.3, 1),
    ([1.1, 1.1, 1.1, 2.2, 2.2, 3.3, 3.3], 1.1, 0),
    ([2.2, 2.2, 3.3, 3.3, 4.4, 4.4, 4.4], 4.4, 4)
]

total_execution_time = 0

# Run the test cases and measure execution time
for array, target, expected_index in test_cases:
    try:
        validate_search_result(array, target, expected_index)
        execution_time = timeit.timeit(lambda: binary_search_float(array, target), number=1000)
        total_execution_time += execution_time
    except Exception as e:
        print(f"Error occurred for test case: array={array}, target={target}, error={str(e)}")
        exit(1)

# Run random test cases with small arrays and measure execution time
random_case_count = 1000
for _ in range(random_case_count):
    try:
        size = random.randint(1, 100)
        array = create_sorted_array(size)
        target = random.choice(array)
        expected_index = array.index(target)
        validate_search_result(array, target, expected_index)
        execution_time = timeit.timeit(lambda: binary_search_float(array, target), number=1000)
        total_execution_time += execution_time
    except Exception as e:
        print(f"Error occurred for test case: array={array}, target={target}, error={str(e)}")
        exit(1)

# Run random test cases with large arrays and measure execution time
random_case_count_large = 1000
for _ in range(random_case_count_large):
    try:
        size = random.randint(1000, 10000)
        array = create_sorted_array(size)
        target = random.choice(array)
        expected_index = array.index(target)
        validate_search_result(array, target, expected_index)
        execution_time = timeit.timeit(lambda: binary_search_float(array, target), number=1000)
        total_execution_time += execution_time
    except Exception as e:
        print(f"Error occurred for test case: array={array}, target={target}, error={str(e)}")
        exit(1)

average_execution_time = total_execution_time / (len(test_cases) + random_case_count + random_case_count_large)
average_execution_time = average_execution_time / 1000  # each case is run 100 times
average_execution_time_ms = average_execution_time * 1000
print(f"Average execution time: {average_execution_time_ms:.10f} ms")