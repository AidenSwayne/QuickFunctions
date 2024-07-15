#include "function.h"

/*
    Parameters:
    arr: a pointer to a vector of possibly empty strings, sorted in ascending lexicographic order, with duplicates.
    target: the target string to search for, which is not guaranteed to be in the array.

    return value:
    the lowest index in arr where target is located if it is present, and -1 otherwise.
*/
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
