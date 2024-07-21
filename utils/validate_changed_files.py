import subprocess
import os
import sys

def validate_changed_files():
    destination_ref=os.environ["MERGE_DESTINATION_REF"]
    source_ref=os.environ["MERGE_SOURCE_REF"]
    source=os.environ["MERGE_SOURCE"]
    subprocess.run(["git", "remote", "add", "fork", source])
    subprocess.run(["git", "fetch", "fork", source_ref])
    subprocess.run(["git", "checkout", "-b", "fork-branch", f"fork/{source_ref}"])
    raw = subprocess.run(["git", "diff","--name-only", f"{destination_ref}..fork-branch"])

    changed_files=str(raw.stdout).strip()
    if raw.stderr!=None:
        print("ERR:"+str(raw.stderr), flush=True, file=sys.stderr)
        raise Exception
    print(f"Changed files: {changed_files}")

    if len(changed_files.split("\n")) != 1:
        return "Only one file should be changed in a pull request."

    is_valid_python = changed_files.startswith("python/") and changed_files.endswith("/function.py")
    is_valid_javascript = changed_files.startswith("javascript/") and changed_files.endswith("/function.js")
    is_valid_cpp = changed_files.startswith("c_plus_plus/") and changed_files.endswith("/function.cpp")

    if not (is_valid_python or is_valid_javascript or is_valid_cpp):
        return "Changed file should be either 'function.py' in a function directory under the 'python' directory, 'function.js' in a function directory under the 'javascript' directory, or 'function.cpp' in a function directory under the 'c_plus_plus' directory."

    return None

error_message = validate_changed_files()
if error_message:
    with open(os.path.join(os.path.dirname(os.path.dirname(os.path.abspath(__file__))), "temp.txt"), "w") as f:
        f.write(error_message)