#include "function.h"

int binary_search_string(const std::vector<std::string>& arr, const std::string& target) {
    int left = 0;
    int right = arr.size() - 1;
    int result = -1;
    

    
    while (left <= right) {
        int mid = (left + right) / 2;
        if (arr[mid] == target) {
            result = mid;
            right = mid - 1;  // Continue searching towards the left
        } else if (arr[mid] < target) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }
    

    return result;
}
