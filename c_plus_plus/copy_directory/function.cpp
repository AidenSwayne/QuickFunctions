#include "function.h"
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

bool copy_directory(const std::string& source_dir, const std::string& destination_dir) {
    try {
        // Use the copy_options::overwrite_existing to avoid deleting the destination directory first
        fs::copy(source_dir, destination_dir, fs::copy_options::recursive | fs::copy_options::overwrite_existing);
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Error copying directory: " << e.what() << std::endl;
        return false;
    }
}