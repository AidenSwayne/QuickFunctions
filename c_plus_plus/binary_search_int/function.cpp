#include "function.h"

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
