from function import copy_directory
import os
import shutil
import tempfile
import timeit
import random
import filecmp

def create_test_files(directory):
    # Create a tree of files and directories
    os.makedirs(os.path.join(directory, "subdir1", "subsubdir1"))
    os.makedirs(os.path.join(directory, "subdir2"))

    # Create files in the directories
    with open(os.path.join(directory, "file1.txt"), "w") as file:
        file.write("This is file1.")

    with open(os.path.join(directory, "subdir1", "file2.txt"), "w") as file:
        file.write("This is file2.")

    with open(os.path.join(directory, "subdir1", "subsubdir1", "file3.txt"), "w") as file:
        file.write("This is file3.")

    with open(os.path.join(directory, "subdir2", "file4.txt"), "w") as file:
        file.write("This is file4.")

    # Create 1MB file with random data
    random.seed(42)  # Fixed seed for reproducibility
    with open(os.path.join(directory, "file_1MB.bin"), "wb") as file:
        file.write(os.urandom(1024 * 1024))  # 1MB

    # Create 200MB file with random data
    random.seed(42)  # Fixed seed for reproducibility
    with open(os.path.join(directory, "file_200MB.bin"), "wb") as file:
        for _ in range(200):
            file.write(os.urandom(1024 * 1024))  # 200MB

def validate_copy_directory(source_dir, destination_dir):
    try:
        result = copy_directory(source_dir, destination_dir)

        if not isinstance(result, bool):
            print(f"Failed test case: source_dir='{source_dir}', destination_dir='{destination_dir}'")
            print(f"Expected return type: bool, Actual return type: {type(result)}")
            exit(1)

        if not result:
            print(f"Failed test case: source_dir='{source_dir}', destination_dir='{destination_dir}'")
            exit(1)

        # Compare the contents of the source and destination directories
        if not filecmp.dircmp(source_dir, destination_dir).diff_files:
            pass
            # print(f"Validation passed for test case: source_dir='{source_dir}', destination_dir='{destination_dir}'")
        else:
            print(f"Failed test case: source_dir='{source_dir}', destination_dir='{destination_dir}'")
            exit(1)
    except Exception as e:
        print(f"Error occurred for test case: source_dir='{source_dir}', destination_dir='{destination_dir}', error={str(e)}")
        exit(1)

# Create a temporary directory to store the test files
temp_test_dir = tempfile.mkdtemp(prefix="test_files_")

# Create the test files in the temporary directory
create_test_files(temp_test_dir)

test_cases = [
    ("source_dir1", "destination_dir1"),
    ("source_dir2", "destination_dir2"),
    ("source_dir3", "destination_dir3"),
]

total_execution_time = 0

# Run the test cases and measure execution time
for source_dir, destination_dir in test_cases:
    # Create a temporary destination directory
    temp_destination_dir = tempfile.mkdtemp()

    # Copy the test files to the source directory
    temp_source_dir = tempfile.mkdtemp()
    shutil.copytree(temp_test_dir, temp_source_dir, dirs_exist_ok=True)

    validate_copy_directory(temp_source_dir, temp_destination_dir)
    execution_time = timeit.timeit(lambda: copy_directory(temp_source_dir, temp_destination_dir), number=100)
    total_execution_time += execution_time

    # Clean up the temporary directories
    shutil.rmtree(temp_source_dir)
    shutil.rmtree(temp_destination_dir)

# Clean up the temporary test files directory
shutil.rmtree(temp_test_dir)

# Run random test cases and measure execution time
random_case_count = 100
for _ in range(random_case_count):
    # Create a temporary destination directory
    temp_destination_dir = tempfile.mkdtemp()

    # Copy the test files to the source directory
    temp_source_dir = tempfile.mkdtemp()
    shutil.copytree(temp_test_dir, temp_source_dir, dirs_exist_ok=True)

    try:
        validate_copy_directory(temp_source_dir, temp_destination_dir)
        execution_time = timeit.timeit(lambda: copy_directory(temp_source_dir, temp_destination_dir), number=100)
        total_execution_time += execution_time
    except Exception as e:
        pass  # Ignore errors for random test cases

    # Clean up the temporary directories
    shutil.rmtree(temp_source_dir)
    shutil.rmtree(temp_destination_dir)

average_execution_time = total_execution_time / (len(test_cases) + random_case_count)
average_execution_time = average_execution_time / 100  # each case is run 100 times
average_execution_time_ms = average_execution_time * 1000
print(f"Average execution time: {average_execution_time_ms:.10f} ms")