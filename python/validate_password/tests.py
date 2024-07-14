from function import validate_password
import timeit
import random
import string

def validate_password_wrapper(password, expected_output):
    try:
        actual_output = validate_password(password)
        if actual_output != expected_output:
            print(f"Failed test case: password='{password}', expected={expected_output}, actual={actual_output}")
            exit(1)
    except Exception as e:
        print(f"Error occurred for test case: password='{password}', error={str(e)}")
        exit(1)

test_cases = [
    ("Abcdefg1!", True),
    ("abcdefgh1", False),
    ("ABCDEFGH!", False),
    ("Abcdefgh!", False),
    ("Abcdefg1", False),
    ("Ab1!", False),
    ("abcdefgh", False),
    ("ABCDEFGH", False),
    ("12345678", False),
    ("!@#$%^&*", False),
    ("Abcdefg1!Abcdefg1!Abcdefg1!Abcdefg1!Abcdefg1!Abcdefg1!Abcdefg1!", True),
    ("Abcdefg1!Abcdefg1!Abcdefg1!Abcdefg1!Abcdefg1!Abcdefg1!Abcdefg1!Abcdefg1!", False),
    ("Abcdefg1!Abcdefg1!Abcdefg1!Abcdefg1!Abcdefg1!Abcdefg1!Abcdef", True),
    ("abcdefghijklmnopqrstuvwxyz1234567890!@#$%^&*(),.?\":{}|<>", False),
    ("ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890!@#$%^&*(),.?\":{}|<>", False),
    ("abcdefgh1!", False),
    ("Abcdefgh1", False),
    ("Abcdefgh!", False),
    ("Abc123!@#", True),
    ("Abc123!", False),
    ("Abc123!@#$%^&*(),.?\":{}|<>", True),
    ("abcABC123!@#", True),
    ("abcABC123!", True),
    ("abcABC123!@#$%^&*(),.?\":{}|<>", True),
]

total_execution_time = 0

# Run the test cases and measure execution time
for password, expected_output in test_cases:
    validate_password_wrapper(password, expected_output)
    execution_time = timeit.timeit(lambda: validate_password(password), number=100)
    total_execution_time += execution_time

# Run random test cases and measure execution time
random_case_count = 1000
for _ in range(random_case_count):
    password = ''.join(random.choices(string.ascii_letters + string.digits + string.punctuation, k=random.randint(5, 70)))
    expected_output = (
        8 <= len(password) <= 64 and
        any(c.isupper() for c in password) and
        any(c.islower() for c in password) and
        any(c.isdigit() for c in password) and
        any(c in string.punctuation for c in password)
    )
    validate_password_wrapper(password, expected_output)
    execution_time = timeit.timeit(lambda: validate_password(password), number=100)
    total_execution_time += execution_time

average_execution_time = total_execution_time / (len(test_cases) + random_case_count)
average_execution_time = average_execution_time / 100  # each case is run 100 times
average_execution_time_ms = average_execution_time * 1000
print(f"Average execution time: {average_execution_time_ms:.10f} ms")