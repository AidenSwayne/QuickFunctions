#include "function.h"
#include <cmath>

/*
    Parameters:
    arr: a vector of valid floats, sorted in ascending order, with duplicates.
    target: the target float to search for, which is not guaranteed to be in the array.

    return value:
    the lowest index in arr where target is located if it is present, and -1 otherwise.
*/
int binary_search_float(const std::vector<double>& arr, double target) {
    int left = 0;
    int right = arr.size() - 1;
    int result = -1;

    while (left <= right) {
        int mid = (left + right) / 2;

        if (std::abs(arr[mid] - target) < 1e-6) {
            result = mid;
            right = mid - 1;
        } else if (arr[mid] < target) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }

    return result;
}
