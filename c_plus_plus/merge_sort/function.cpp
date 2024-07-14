#include "function.h"

std::vector<int> merge_sort(std::vector<int> arr) {
    if (arr.size() <= 1) {
        return arr;
    }

    size_t mid = arr.size() / 2;
    std::vector<int> left(arr.begin(), arr.begin() + mid);
    std::vector<int> right(arr.begin() + mid, arr.end());

    left = merge_sort(left);
    right = merge_sort(right);

    std::vector<int> result;
    size_t i = 0, j = 0;

    while (i < left.size() && j < right.size()) {
        if (left[i] <= right[j]) {
            result.push_back(left[i]);
            i++;
        } else {
            result.push_back(right[j]);
            j++;
        }
    }

    result.insert(result.end(), left.begin() + i, left.end());
    result.insert(result.end(), right.begin() + j, right.end());

    return result;
}