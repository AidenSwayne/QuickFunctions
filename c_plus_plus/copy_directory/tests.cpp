#include "function.h"
#include <iostream>
#include <fstream>
#include <chrono>
#include <random>
#include <filesystem>
#include <vector>

namespace fs = std::filesystem;

void create_test_files(const std::string& directory) {
    std::cout.setf(std::ios::unitbuf);
    std::cout << "Began creating the test files";
    fs::create_directories(fs::path(directory) / "subdir1" / "subsubdir1");
    fs::create_directories(fs::path(directory) / "subdir2");

    std::ofstream(fs::path(directory) / "file1.txt") << "This is file1.";
    std::ofstream(fs::path(directory) / "subdir1" / "file2.txt") << "This is file2.";
    std::ofstream(fs::path(directory) / "subdir1" / "subsubdir1" / "file3.txt") << "This is file3.";
    std::ofstream(fs::path(directory) / "subdir2" / "file4.txt") << "This is file4.";

    std::random_device rd;
    std::mt19937 gen(42);  // Fixed seed for reproducibility
    std::uniform_int_distribution<> dis(0, 255);

    std::ofstream file_1MB(fs::path(directory) / "file_1MB.bin", std::ios::binary);
    for (int i = 0; i < 1024 * 1024; ++i) {
        file_1MB.put(static_cast<char>(dis(gen)));
    }

    std::ofstream file_200MB(fs::path(directory) / "file_200MB.bin", std::ios::binary);
    for (int i = 0; i < 200 * 1024 * 1024; ++i) {
        file_200MB.put(static_cast<char>(dis(gen)));
    }
    std::cout << "Finished creating the test files";
}

bool compare_directories(const fs::path& dir1, const fs::path& dir2) {
    auto dir1_iter = fs::recursive_directory_iterator(dir1);
    auto dir2_iter = fs::recursive_directory_iterator(dir2);

    while (dir1_iter != fs::recursive_directory_iterator{} && dir2_iter != fs::recursive_directory_iterator{}) {
        if (dir1_iter->path().lexically_relative(dir1) != dir2_iter->path().lexically_relative(dir2)) {
            return false;
        }
        if (fs::is_regular_file(dir1_iter->status()) && fs::is_regular_file(dir2_iter->status())) {
            if (fs::file_size(dir1_iter->path()) != fs::file_size(dir2_iter->path())) {
                return false;
            }
        }
        ++dir1_iter;
        ++dir2_iter;
    }
    return dir1_iter == fs::recursive_directory_iterator{} && dir2_iter == fs::recursive_directory_iterator{};
}

void validate_copy_directory(const std::string& source_dir, const std::string& destination_dir) {
    bool result = copy_directory(source_dir, destination_dir);

    if (!result) {
        std::cout << "Failed test case: source_dir='" << source_dir << "', destination_dir='" << destination_dir << "'" << std::endl;
        exit(1);
    }

    if (!compare_directories(source_dir, destination_dir)) {
        std::cout << "Failed test case: source_dir='" << source_dir << "', destination_dir='" << destination_dir << "'" << std::endl;
        exit(1);
    }
}

int main() {
    std::cout.setf(std::ios::unitbuf);
    fs::path temp_test_dir = fs::temp_directory_path() / "test_files_";
    fs::create_directories(temp_test_dir);

    create_test_files(temp_test_dir.string());

    std::vector<std::pair<std::string, std::string>> test_cases = {
        {"source_dir1", "destination_dir1"},
        {"source_dir2", "destination_dir2"},
        {"source_dir3", "destination_dir3"},
    };

    double total_execution_time = 0.0;
    std::cout << "Began running the validation test cases";
    for (const auto& [source_dir, destination_dir] : test_cases) {
        fs::path temp_source_dir = fs::temp_directory_path() / source_dir;
        fs::path temp_destination_dir = fs::temp_directory_path() / destination_dir;

        fs::create_directories(temp_source_dir);
        fs::copy(temp_test_dir, temp_source_dir, fs::copy_options::recursive);

        validate_copy_directory(temp_source_dir.string(), temp_destination_dir.string());

        auto start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < 100; ++i) {
            copy_directory(temp_source_dir.string(), temp_destination_dir.string());
        }
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> diff = end - start;
        total_execution_time += diff.count();

        fs::remove_all(temp_source_dir);
        fs::remove_all(temp_destination_dir);
    }
    std::cout << "Finished running the validation test cases";
    int random_case_count = 100;
    for (int i = 0; i < random_case_count; ++i) {
        std::cout << "Began a random test case loop";
        fs::path temp_source_dir = fs::temp_directory_path() / ("random_source_" + std::to_string(i));
        fs::path temp_destination_dir = fs::temp_directory_path() / ("random_destination_" + std::to_string(i));

        fs::create_directories(temp_source_dir);
        fs::copy(temp_test_dir, temp_source_dir, fs::copy_options::recursive);

        try {
            validate_copy_directory(temp_source_dir.string(), temp_destination_dir.string());

            auto start = std::chrono::high_resolution_clock::now();
            for (int j = 0; j < 100; ++j) {
                copy_directory(temp_source_dir.string(), temp_destination_dir.string());
            }
            auto end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> diff = end - start;
            total_execution_time += diff.count();
        } catch (const std::exception& e) {
            // Ignore errors for random test cases
        }

        fs::remove_all(temp_source_dir);
        fs::remove_all(temp_destination_dir);
        std::cout << "Finished a random test case loop";
    }

    fs::remove_all(temp_test_dir);

    double average_execution_time = total_execution_time / (test_cases.size() + random_case_count);
    average_execution_time = average_execution_time / 100.0;  // each case is run 100 times
    double average_execution_time_ms = average_execution_time * 1000.0;
    std::cout << std::fixed << std::setprecision(10);
    std::cout << "Average execution time: " << average_execution_time_ms << " ms" << std::endl;

    return 0;
}