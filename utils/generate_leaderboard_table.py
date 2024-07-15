import os
import subprocess
import re

def get_repo_root():
    return subprocess.check_output(['git', 'rev-parse', '--show-toplevel']).decode('utf-8').strip()

def read_top_time(file_path):
    if os.path.exists(file_path):
        with open(file_path, 'r') as f:
            first_line = f.readline().strip()
            parts = first_line.split(" ")
            if len(parts) >= 2 and parts[1] == 'ms':
                return f"{parts[0]} ms"
            else:
                return None
    else:
        return None

def generate_table():
    table = '''### Current records
| Function | JavaScript | Python | C++ |
|----------|------------|--------|-----|
| Binary Search (Float) | [js_time](./javascript/binarySearchFloat/function.js) | [py_time](./python/binary_search_float/function.py) | [cpp_time](./c_plus_plus/binary_search_float/function.cpp) |
| Binary Search (Int) | [js_time](./javascript/binarySearchInt/function.js) | [py_time](./python/binary_search_int/function.py) | [cpp_time](./c_plus_plus/binary_search_int/function.cpp) |
| Binary Search (String) | [js_time](./javascript/binarySearchString/function.js) | [py_time](./python/binary_search_string/function.py) | [cpp_time](./c_plus_plus/binary_search_string/function.cpp) |
| JSON Encoder | [js_time](./javascript/jsonEncoder/function.js) | [py_time](./python/json_encoder/function.py) | [cpp_time](./c_plus_plus/json_encoder/function.cpp) |
| Matrix Multiply 2D | [js_time](./javascript/matrixMultiply2d/function.js) | [py_time](./python/matrix_multiply_2d/function.py) | [cpp_time](./c_plus_plus/matrix_multiply_2d/function.cpp) |
| Merge Sort | [js_time](./javascript/mergeSort/function.js) | [py_time](./python/merge_sort/function.py) | [cpp_time](./c_plus_plus/merge_sort/function.cpp) |
| Quicksort | [js_time](./javascript/quicksort/function.js) | [py_time](./python/quicksort/function.py) | [cpp_time](./c_plus_plus/quicksort/function.cpp) |
| Validate Password | [js_time](./javascript/validatePassword/function.js) | [py_time](./python/validate_password/function.py) | [cpp_time](./c_plus_plus/validate_password/function.cpp) |
'''
    functions = [
        'binary_search_float', 'binary_search_int', 'binary_search_string',
        'json_encoder', 'matrix_multiply_2d',
        'merge_sort', 'quicksort', 'validate_password'
    ]
    
    js_functions = [
        'binarySearchFloat', 'binarySearchInt', 'binarySearchString',
        'jsonEncoder', 'matrixMultiply2d',
        'mergeSort', 'quicksort', 'validatePassword'
    ]
    
    repo_root = get_repo_root()
    
    for lang in ['js', 'py', 'cpp']:
        for i, func in enumerate(functions):
            if lang == 'js':
                folder = 'javascript'
                file_name = js_functions[i]
            elif lang == 'py':
                folder = 'python'
                file_name = func
            else:
                folder = 'c_plus_plus'
                file_name = func
            
            leaderboard_path = os.path.join(repo_root, folder, file_name, 'leaderboard.txt')
            time = read_top_time(leaderboard_path)
            if time:
                table = table.replace(f'[{lang}_time]', f'[{time}]', 1)
            else:
                print("FAILED TO READ TIME FOR", leaderboard_path)
                table = table.replace(f'[{lang}_time]', '[N/A]', 1)
    return table

def update_readme(new_table):
    readme_path = os.path.join(get_repo_root(), 'README.md')
    with open(readme_path, 'r') as f:
        content = f.read()
    
    # Find the start of the current table
    table_start = content.find('### Current records')
    if table_start == -1:
        print("Could not find the table in README.md")
        return
    
    # Find the end of the current table
    table_end = content.find('\n\n', table_start)
    if table_end == -1:
        table_end = len(content)
    
    # Replace the old table with the new one
    updated_content = content[:table_start] + new_table.strip() + content[table_end:]
    
    with open(readme_path, 'w') as f:
        f.write(updated_content)

new_table = generate_table()
update_readme(new_table)
print("README.md has been updated with the new table.")