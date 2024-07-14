#include "function.h"
#include <sstream>
#include <iomanip>
#include <functional>
#include <map>
#include <vector>
#include <set>
#include <complex>
#include <cmath>

std::string json_encode(const std::any& data) {
    std::ostringstream oss;
    
    std::function<void(const std::any&)> encode = [&](const std::any& obj) {
        if (obj.type() == typeid(std::map<std::string, std::any>)) {
            const auto& map = std::any_cast<const std::map<std::string, std::any>&>(obj);
            oss << '{';
            bool first = true;
            for (const auto& [key, value] : map) {
                if (!first) oss << ',';
                oss << '"' << key << "\":";
                encode(value);
                first = false;
            }
            oss << '}';
        } else if (obj.type() == typeid(std::vector<std::any>)) {
            const auto& vec = std::any_cast<const std::vector<std::any>&>(obj);
            oss << '[';
            bool first = true;
            for (const auto& item : vec) {
                if (!first) oss << ',';
                encode(item);
                first = false;
            }
            oss << ']';
        } else if (obj.type() == typeid(std::set<int>)) {
            const auto& set = std::any_cast<const std::set<int>&>(obj);
            oss << '[';
            bool first = true;
            for (const auto& item : set) {
                if (!first) oss << ',';
                oss << item;
                first = false;
            }
            oss << ']';
        } else if (obj.type() == typeid(std::complex<double>)) {
            auto c = std::any_cast<std::complex<double>>(obj);
            oss << '[' << c.real() << ',' << c.imag() << ']';
        } else if (obj.type() == typeid(double)) {
            double d = std::any_cast<double>(obj);
            if (std::isinf(d)) {
                oss << (d > 0 ? "\"Infinity\"" : "\"-Infinity\"");
            } else if (std::isnan(d)) {
                oss << "\"NaN\"";
            } else {
                oss << std::setprecision(17) << d;
            }
        } else if (obj.type() == typeid(int)) {
            oss << std::any_cast<int>(obj);
        } else if (obj.type() == typeid(std::string)) {
            const auto& s = std::any_cast<const std::string&>(obj);
            oss << '"';
            for (char c : s) {
                switch (c) {
                    case '"': oss << "\\\""; break;
                    case '\\': oss << "\\\\"; break;
                    case '\b': oss << "\\b"; break;
                    case '\f': oss << "\\f"; break;
                    case '\n': oss << "\\n"; break;
                    case '\r': oss << "\\r"; break;
                    case '\t': oss << "\\t"; break;
                    default:
                        if ('\x00' <= c && c <= '\x1f') {
                            oss << "\\u" << std::hex << std::setw(4) << std::setfill('0') << static_cast<int>(c);
                        } else {
                            oss << c;
                        }
                }
            }
            oss << '"';
        } else {
            oss << "null";
        }
    };
    
    encode(data);
    return oss.str();
}
