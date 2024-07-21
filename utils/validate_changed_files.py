import subprocess
import os
import sys

def validate_changed_files():
    import os
    import subprocess

    def run_command(cmd, capture=True):
        result = subprocess.run(cmd, capture_output=capture, text=True)
        if result.returncode != 0:
            print(f"Error running command: {' '.join(cmd)}")
            print(f"stderr: {result.stderr}")
        return result

    source_ref = os.environ["MERGE_SOURCE_REF"]
    source = os.environ["MERGE_SOURCE"]
    destination_sha = os.environ["MERGE_DESTINATION_SHA"]

    print(f"Source Ref: {source_ref}")
    print(f"Source: {source}")
    print(f"Destination SHA: {destination_sha}")

    run_command(["git", "remote", "add", "fork", source])
    run_command(["git", "fetch", "fork", source_ref])

    print("Current branches:")
    run_command(["git", "branch", "-a"], capture=False)

    print("Recent commit history:")
    run_command(["git", "log", "--oneline", "--graph", "--all", "-n", "20"], capture=False)

    raw1 = run_command(["git", "merge-base", destination_sha, "FETCH_HEAD"])
    destination_ref = raw1.stdout.strip()

    print(f"Base commit (merge-base result): {destination_ref}")

    print("Diff between base and FETCH_HEAD:")
    raw = run_command(["git", "diff", "--name-only", destination_ref, "FETCH_HEAD"])
    changed_files = raw.stdout.strip().split('\n')

    if changed_files:
        print("Changed files:")
        for file in changed_files:
            print(file)
    else:
        print("No changes detected")

    print("Full diff:")
    run_command(["git", "diff", destination_ref, "FETCH_HEAD"], capture=False)

    run_command(["git", "checkout", "-b", "fork-branch", f"fork/{source_ref}"])

    changed_files=raw.stdout.strip()
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