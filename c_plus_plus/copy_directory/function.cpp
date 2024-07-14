#include "function.h"
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

bool copy_directory(const std::string& source_dir, const std::string& destination_dir) {
    try {
        // Delete the destination directory if it already exists
        if (fs::exists(destination_dir)) {
            fs::remove_all(destination_dir);
        }

        // Copy the source directory to the destination directory
        fs::copy(source_dir, destination_dir, fs::copy_options::recursive);
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Error copying directory: " << e.what() << std::endl;
        return false;
    }
}