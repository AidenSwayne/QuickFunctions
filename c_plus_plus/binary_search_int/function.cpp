#include "function.h"

/*
    Parameters:
    arr: a vector of integers, sorted in ascending order, with duplicates.
    target: the target integer to search for, which is not guaranteed to be in the array.

    return value:
    the lowest index in arr where target is located if it is present, and -1 otherwise.
*/

int binary_search_int(const std::vector<int>& arr, int target) {
    int left = 0;
    int right = arr.size() - 1;
    int result = -1;
    

    while (left <= right) {
        int mid = (left + right) / 2;

        if (arr[mid] == target) {
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
