import subprocess
import os
import sys

def validate_changed_files():
    source_ref=os.environ["MERGE_SOURCE_REF"]
    source=os.environ["MERGE_SOURCE"]

    subprocess.run(["git", "remote", "add", "fork", source])
    subprocess.run(["git", "fetch", "fork", source_ref])
    raw1=subprocess.run(["git","merge-base",os.environ["MERGE_DESTINATION_SHA"],"FETCH_HEAD"],capture_output=True)
    destination_ref=raw1.stdout.decode().strip("\n")
    subprocess.run(["git", "checkout", "-b", "fork-branch", f"fork/{source_ref}"])
    raw = subprocess.run(["git", "diff","--name-only", destination_ref,"fork-branch"], capture_output=True, text=True)
    changed_files=raw.stdout.strip("\n")
    print(changed_files)
    if raw.returncode!=0:
        print("ERR:"+raw.stderr, flush=True, file=sys.stderr)
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