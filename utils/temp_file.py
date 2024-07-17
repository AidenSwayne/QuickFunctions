import os
import subprocess
import sys

def run_command(command):
    process = subprocess.Popen(command, stdout=subprocess.PIPE, stderr=subprocess.PIPE, shell=True, text=True)
    output, error = process.communicate()
    if process.returncode != 0:
        print(f"Error executing command: {command}")
        print(f"Error message: {error}")
        sys.exit(1)
    return output.strip()

def main():
    # Ensure we're in a git repository
    if not os.path.isdir('.git'):
        print("This script must be run from the root of a Git repository.")
        sys.exit(1)

    # Fetch the latest changes
    run_command("git fetch origin")

    # Get the current branch name
    current_branch = run_command("git rev-parse --abbrev-ref HEAD")

    # Get the name of the main branch (assuming it's 'main', adjust if necessary)
    main_branch = "main"

    # Find differences
    diff_output = run_command(f"git diff origin/{main_branch}...{current_branch} --name-only")

    if not diff_output:
        print("No differences found between the current branch and main.")
        return

    changed_files = diff_output.split('\n')
    print("Changed files:")
    for i, file in enumerate(changed_files, 1):
        print(f"{i}. {file}")

    # If there's only one changed file, use that. Otherwise, ask the user to choose.
    if len(changed_files) == 1:
        chosen_file = changed_files[0]
    else:
        while True:
            try:
                choice = int(input("Enter the number of the file to switch to: "))
                if 1 <= choice <= len(changed_files):
                    chosen_file = changed_files[choice - 1]
                    break
                else:
                    print("Invalid choice. Please try again.")
            except ValueError:
                print("Please enter a valid number.")

    # Get the directory of the chosen file
    file_dir = os.path.dirname(chosen_file)

    # Change to the directory of the changed file
    if file_dir:
        os.chdir(file_dir)
        print(f"Changed working directory to: {os.getcwd()}")
    else:
        print("The changed file is in the root directory. Working directory unchanged.")

if __name__ == "__main__":
    main()