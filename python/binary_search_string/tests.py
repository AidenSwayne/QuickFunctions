from function import binary_search_string
import timeit
import random
import string

def create_sorted_array(size: int, min_length: int = 1, max_length: int = 10) -> list[str]:
    return sorted([''.join(random.choices(string.ascii_letters, k=random.randint(min_length, max_length))) for _ in range(size)])

def validate_search_result(array: list[str], target: str, expected_index: int) -> None:
    index = binary_search_string(array, target)
    if index != expected_index:
        print(f"Failed test case: array={array}, target={target}")
        print(f"Expected index: {expected_index}")
        print(f"Actual index: {index}")
        exit(1)

test_cases = [
    (["apple", "banana", "cherry", "date", "elderberry"], "cherry", 2),
    (["cat", "dog", "elephant", "fox", "giraffe"], "horse", -1),
    (["alice", "bob", "charlie", "david", "eve"], "alice", 0),
    (["alpha", "beta", "beta", "gamma", "gamma", "gamma", "delta"], "gamma", 3),
    (['blue', 'green', 'indigo', 'orange', 'red', 'yellow'], "orange", 3),
    (["a", "a", "a", "b", "b", "c", "c"], "a", 0),
    (["x", "x", "y", "y", "z", "z", "z"], "z", 4)
]

total_execution_time = 0

# Run the test cases and measure execution time
for array, target, expected_index in test_cases:
    try:
        validate_search_result(array, target, expected_index)
        execution_time = timeit.timeit(lambda: binary_search_string(array, target), number=100)
        total_execution_time += execution_time
    except Exception as e:
        print(f"Error occurred for test case: array={array}, target={target}, error={str(e)}")
        exit(1)

# Run random test cases with small arrays and measure execution time
random_case_count = 100
for _ in range(random_case_count):
    try:
        size = random.randint(1, 100)
        array = create_sorted_array(size)
        target = random.choice(array)
        expected_index = array.index(target)
        validate_search_result(array, target, expected_index)
        execution_time = timeit.timeit(lambda: binary_search_string(array, target), number=100)
        total_execution_time += execution_time
    except Exception as e:
        print(f"Error occurred for test case: array={array}, target={target}, error={str(e)}")
        exit(1)

# Run random test cases with large arrays and measure execution time
random_case_count_large = 10
for _ in range(random_case_count_large):
    try:
        size = random.randint(1000, 10000)
        array = create_sorted_array(size)
        target = random.choice(array)
        expected_index = array.index(target)
        validate_search_result(array, target, expected_index)
        execution_time = timeit.timeit(lambda: binary_search_string(array, target), number=100)
        total_execution_time += execution_time
    except Exception as e:
        print(f"Error occurred for test case: array={array}, target={target}, error={str(e)}")
        exit(1)

average_execution_time = total_execution_time / (len(test_cases) + random_case_count + random_case_count_large)
average_execution_time = average_execution_time / 100  # each case is run 100 times
average_execution_time_ms = average_execution_time * 1000
print(f"Average execution time: {average_execution_time_ms:.10f} ms")