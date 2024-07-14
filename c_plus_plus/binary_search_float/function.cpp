#include "function.h"
#include <cmath>


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
