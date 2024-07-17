print("Began file run")
import os
import subprocess
import re
import datetime

def run_tests_and_validate_output(source, destination):
    try:
        subprocess.run(["git", "remote", "add", "-f", "source",source])
        subprocess.run(["git", "remote", "add", "-f", "destination",destination])
        changed_files = subprocess.run(["git", "diff", "--name-only", "remotes/source/master", "remotes/destination/master"], capture_output=True, text=True).stdout.strip()
        file_path = changed_files.split("\n")[0]
        function_dir = os.path.dirname(file_path)
        print("Filepath: "+file_path)
        os.chdir(os.path.join(os.environ["GITHUB_WORKSPACE"], function_dir))
        if file_path.endswith('.py'):
            output = subprocess.run(["python", "tests.py"], capture_output=True, text=True).stdout.strip()
        elif file_path.endswith('.js'):
            output = subprocess.run(["node", "tests.js"], capture_output=True, text=True).stdout.strip()
        elif file_path.endswith('.cpp'):
            compile_result = subprocess.run(["g++", "-o", "tests", "tests.cpp", "function.cpp"], capture_output=True, text=True)
            if compile_result.returncode != 0:
                return {"error": f"C++ compilation failed:\n{compile_result.stderr}"}
            if os.path.exists("./tests"):
                output = subprocess.run(["./tests"], capture_output=True, text=True).stdout.strip()
            else:
                return {"error": "C++ test executable not found after compilation."}
        else:
            return {"error": "Invalid file type. Only .py, .js, and .cpp files are supported."}
        if "Failed" in output:
            failing_test_case_match = re.search(r"Failed test case: (.*)", output)
            if failing_test_case_match:
                failing_test_case = failing_test_case_match.group(1)
                return {"error": f"Implementation failed one or more test cases:\n{failing_test_case}"}
            else:
                return {"error": "Implementation failed one or more test cases."}
        print("Full output: "+output)
        execution_time_match = re.search(r"Average execution time: ([\d.]+)", output)
        if execution_time_match:
            execution_time = execution_time_match.group(1)
        else:
            return {"error": f"Unable to find execution time in the output. Here is the full file output: {output}"}

        is_new_record = False
        percentage_improvement = 0.0

        current_datetime = datetime.datetime.now().strftime("%Y/%m/%d %H:%M")

        pr_author = os.environ["GITHUB_ACTOR"]

        if os.path.isfile("leaderboard.txt"):
            with open("leaderboard.txt", "r") as f:
                lines = f.readlines()
                if lines:
                    first_line = lines[0].strip()
                    if first_line:
                        best_time = float(first_line.split("      ")[0].replace(" ms", ""))

                        if float(execution_time) < best_time:
                            with open("leaderboard.txt", "w") as f:
                                # Write new record
                                f.write(f"{execution_time} ms      {current_datetime}      {pr_author}\n")
                                for line in lines:
                                    f.write(line.replace("\t", "      ", 1).replace("\t", "      "))

                            percentage_improvement = (best_time - float(execution_time)) / best_time * 100
                            is_new_record = True
                        else:
                            return {"no-record": f"Not a new record.\n\nPercent slowdown: "+str(((float(execution_time) - best_time)/best_time) * 100)+"%"}
                    else:
                        with open("leaderboard.txt", "w") as f:
                            f.write(f"{execution_time} ms      {current_datetime}      {pr_author}\n")
                        is_new_record = True
                else:
                    with open("leaderboard.txt", "w") as f:
                        f.write(f"{execution_time} ms      {current_datetime}      {pr_author}\n")
                    is_new_record = True
        else:
            with open("leaderboard.txt", "w") as f:
                f.write(f"{execution_time} ms      {current_datetime}      {pr_author}\n")
            is_new_record = True

        return {
            "execution_time": execution_time,
            "is_new_record": is_new_record,
            "percentage_improvement": percentage_improvement
        }
    except Exception as e:
        return {"error": f"An error occurred:\n{str(e)}"}

def generate_comment(execution_time, is_new_record, percentage_improvement):
    comment_body = f"Pull request merged successfully!\n\nAverage execution time: {execution_time} ms"

    if is_new_record:
        print(percentage_improvement)
        comment_body += f"\n\nCongratulations! You achieved a new record time with an improvement of {percentage_improvement:.2f}%!"

    return comment_body
source_URL=os.environ["MERGE_SOURCE_URL"]
destination_URL=os.environ["GITHUB_CONTEXT"]
result = run_tests_and_validate_output(source_URL, destination_URL)
if "error" in result:
    print(result["error"])
elif "no-record" in result:
    print(result["no-record"])
else:
    comment = generate_comment(result["execution_time"], result["is_new_record"], result["percentage_improvement"])
    print(comment)