#include "function.h"
#include <algorithm>

bool validate_password(const std::string& password) {
    if (password.length() < 8 || password.length() > 64) {
        return false;
    }
    
    bool has_uppercase = false;
    bool has_lowercase = false;
    bool has_digit = false;
    bool has_special = false;
    
    const std::string special_chars = "`~!@#$%^&*()-_=+[]{}\|;:'\",./<>?";
    
    for (char c : password) {
        if (std::isupper(c)) {
            has_uppercase = true;
        } else if (std::islower(c)) {
            has_lowercase = true;
        } else if (std::isdigit(c)) {
            has_digit = true;
        } else if (special_chars.find(c) != std::string::npos) {
            has_special = true;
        }
    }
    
    return has_uppercase && has_lowercase && has_digit && has_special;
}