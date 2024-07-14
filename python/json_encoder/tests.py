from function import json_encode
import timeit
import random
import json
import math

def validate_json_encode(input_data, expected_output):
    try:
        actual_output = json_encode(input_data)
        if not isinstance(actual_output, str):
            print(f"Failed test case: input='{input_data}'")
            print(f"Expected return type: str, Actual return type: {type(actual_output)}")
            exit(1)
        if actual_output != expected_output:
            print(f"Failed test case: input='{input_data}', expected={expected_output}, actual={actual_output}")
            exit(1)
    except Exception as e:
        print(f"Error occurred for test case: input='{input_data}', error={str(e)}")
        exit(1)

test_cases = [
    ({"name": "John", "age": 30}, '{"name": "John", "age": 30}'),
    ([1, 2, 3], '[1, 2, 3]'),
    ({"set": {1, 2, 3}}, '{"set": [1, 2, 3]}'),
    ({"complex": 1 + 2j}, '{"complex": [1.0, 2.0]}'),
    ({"invalid": float('inf')}, '{"invalid": Infinity}'),
    ({"invalid": float('-inf')}, '{"invalid": -Infinity}'),
    ({"invalid": float('nan')}, '{"invalid": NaN}'),
]

total_execution_time = 0

# Run the test cases and measure execution time
for input_data, expected_output in test_cases:
    validate_json_encode(input_data, expected_output)
    execution_time = timeit.timeit(lambda: json_encode(input_data), number=1000)
    total_execution_time += execution_time

# Run random test cases and measure execution time
random_case_count = 100
for _ in range(random_case_count):
    input_data = random.choice([
        {"key": "value", "nested": {"key": "value"}},
        [random.randint(1, 100) for _ in range(random.randint(1, 10))],
        {"set": set(random.randint(1, 100) for _ in range(random.randint(1, 10)))},
        {"complex": random.randint(1, 100) + random.randint(1, 100) * 1j},
    ])

    def custom_json_encoder(obj):
        if isinstance(obj, complex):
            return [obj.real, obj.imag]
        elif isinstance(obj, set):
            return list(obj)
        elif obj == float('inf'):
            return "Infinity"
        elif obj == float('-inf'):
            return "-Infinity"
        elif isinstance(obj, float) and math.isnan(obj):
            return "NaN"
        return obj

    expected_output = json.dumps(input_data, default=custom_json_encoder)
    validate_json_encode(input_data, expected_output)
    execution_time = timeit.timeit(lambda: json_encode(input_data), number=1000)
    total_execution_time += execution_time

average_execution_time = total_execution_time / (len(test_cases) + random_case_count)
average_execution_time = average_execution_time / 1000  # each case is run 1000 times
average_execution_time_ms = average_execution_time * 1000
print(f"Average execution time: {average_execution_time_ms:.10f} ms")